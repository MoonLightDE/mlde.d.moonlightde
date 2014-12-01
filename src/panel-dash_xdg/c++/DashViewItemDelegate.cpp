/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Jorge Alberto Díaz Orozco
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
#include "DashViewItemDelegate.h"

#include <QApplication>
#include <QDebug>
#include <QState>
#include <QPainter>

DashViewItemDelegate::DashViewItemDelegate() {
}

DashViewItemDelegate::~DashViewItemDelegate() {
}

void DashViewItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    //    QStyledItemDelegate::paint(painter, option, index);
    QString name = index.model()->data(index, Qt::DisplayRole).toString();
    QIcon icon = index.model()->data(index, Qt::DecorationRole).value<QIcon>();
    QRect iconRect = QRect(option.rect.x() + 10, option.rect.y(), option.rect.width() - 20, option.rect.height() - 40);
    QRect textRect = QRect(option.rect.x(), option.rect.y() + 60, option.rect.width(), option.rect.height() - 60);

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    icon.paint(painter, iconRect, Qt::AlignCenter | Qt::AlignTop);
    painter->drawText(textRect, Qt::TextWordWrap | Qt::TextWrapAnywhere | Qt::AlignCenter | Qt::TextEditable, name);
    painter->save();
}

QSize DashViewItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const {
    return QSize(80, 100);
}