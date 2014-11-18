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

DashViewItemDelegate::DashViewItemDelegate() {
}

DashViewItemDelegate::~DashViewItemDelegate() {
}

//void DashViewItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
//    QStyledItemDelegate::paint(painter, option, index);
//}

QSize DashViewItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const {
    return QSize(65,65);
}