/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *  Yosmay Morales Suarez
 * 
 * This file is part of Moonlight Desktop Environment.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */

#include "IconDelegate.h"
#include <QSize>
#include <QPainter>
#include <QIcon>
#include <QStyleOptionViewItem>
#include <QApplication>
#include <QTextLayout>
#include <QTextLine>
#include <QMouseEvent>
#include <QDebug>

ItemDelegate::ItemDelegate(QObject*parent) : QAbstractItemDelegate(parent), m_IconSize(48, 48) {
    m_TextAreaWidth = (m_IconSize.width()) * 2;
    m_Paddings = m_IconSize.width() / 4;
    m_MaxTextLines = 3;
}

ItemDelegate::~ItemDelegate() {
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)
const {
    //    QSize iconSize = option.decorationSize;
    QFontMetrics fm(option.fontMetrics);

    QString txt = index.data(Qt::DisplayRole).toString();
    QPixmap icon = index.data(Qt::DecorationRole).value<QIcon>().pixmap(m_IconSize, QIcon::Normal);

    int textLines = std::min(std::max(fm.width(txt) / m_TextAreaWidth, 1), 3);
    QSize s_retorno(m_TextAreaWidth + m_Paddings * 2, icon.height() + m_Paddings * 2 + (fm.height() + fm.lineSpacing()) * textLines);
    return s_retorno;
}

void ItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QRect optionRect(option.rect);

    optionRect.setX(option.rect.x() - m_Paddings);
    // Font Mentrics for elided text;

    // Basic Painter Settings
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    QString txt = index.data(Qt::DisplayRole).toString();

    QPixmap icon;
    if (option.state & QStyle::State_Selected) {
        icon = index.data(Qt::DecorationRole).value<QIcon>().pixmap(m_IconSize, QIcon::Selected, QIcon::On);

    } else if (option.state & QStyle::State_MouseOver)
        icon = index.data(Qt::DecorationRole).value<QIcon>().pixmap(m_IconSize, QIcon::Selected);

    else
        icon = index.data(Qt::DecorationRole).value<QIcon>().pixmap(m_IconSize, QIcon::Normal);

    if ((icon.size().width() > m_IconSize.width()) or ( icon.size().height() > m_IconSize.height()))
        icon = icon.scaled(m_IconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QRect iconRect;
    // Original X
    iconRect.setX(optionRect.x() + (optionRect.width() - m_IconSize.width()) / 2);
    // Original Y + Image Top Border + Height to make the image center of the icon rectangle
    iconRect.setY((optionRect.y() + m_Paddings + (m_IconSize.height() - m_IconSize.height()) / 2) - 10);
    // Icon Size
    iconRect.setSize(m_IconSize);

    // Paint Icon
    painter->drawPixmap(iconRect, icon);

    QRect textRect;
    // Horizontal Centering, so don't bother
    textRect.setX(optionRect.x() + m_Paddings);
    // Original Y + Image Height + Image Padding Top + Text-Image Padding ( max = 3 )
    textRect.setY((optionRect.y() + m_IconSize.height() + m_Paddings));
    // Left and Right Border
    textRect.setSize(optionRect.size() - QSize(m_Paddings * 2, 0));

    QString newText = painter->fontMetrics().elidedText(txt, Qt::ElideRight, m_TextAreaWidth * m_MaxTextLines);
    painter->drawText(textRect, Qt::AlignHCenter | Qt::TextWrapAnywhere, newText);

    if (option.state & QStyle::State_Selected) {
        //painter->fillRect(option.rect,option.palette.foreground() );

        QPainterPath roundRectPath;
        painter->drawPixmap(iconRect, icon);
        QRect fill(textRect);
        fill.setWidth(fill.width() + m_Paddings);
        fill.setX(fill.x() - m_Paddings);

        if (painter->fontMetrics().boundingRect(newText).width() < textRect.width()) {
            int rest = (textRect.width() - painter->fontMetrics().boundingRect(newText).width()) / 2 - 5;
            fill.setLeft(textRect.left() + rest);
            fill.setRight(textRect.right() - rest);
            fill.setHeight(painter->fontMetrics().boundingRect(newText).height());
            painter->drawText(fill, Qt::AlignHCenter | Qt::TextWrapAnywhere, newText);
        } else if (painter->fontMetrics().boundingRect(newText).width() >= textRect.width() && painter->fontMetrics().boundingRect(newText).width() < 2 * textRect.width()) {
            fill.setHeight(painter->fontMetrics().boundingRect(newText).height()*2);
            painter->drawText(fill, Qt::AlignHCenter | Qt::TextWrapAnywhere, newText);
        } else {
            fill.setHeight(painter->fontMetrics().boundingRect(newText).height()*3);
            painter->drawText(fill, Qt::AlignHCenter | Qt::TextWrapAnywhere, newText);
        }

        roundRectPath.addRoundRect(fill, 15, 30);
        painter->fillPath(roundRectPath, option.palette.highlight());
        painter->drawText(textRect, Qt::AlignHCenter | Qt::TextWrapAnywhere, newText);
    }

}
