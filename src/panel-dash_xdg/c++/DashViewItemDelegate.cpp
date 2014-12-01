/* 
 * File:   DashViewItemDelegate.cpp
 * Author: akiel
 * 
 * Created on November 18, 2014, 12:54 PM
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