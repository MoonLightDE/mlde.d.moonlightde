/* 
 * File:   DashViewItemDelegate.h
 * Author: akiel
 *
 * Created on November 18, 2014, 12:54 PM
 */

#ifndef DASHVIEWITEMDELEGATE_H
#define	DASHVIEWITEMDELEGATE_H

#include <QStyledItemDelegate>

class DashViewItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    DashViewItemDelegate();
    virtual ~DashViewItemDelegate();
    void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const ;
    QSize sizeHint(const QStyleOptionViewItem & , const QModelIndex &) const ;
private:

};

#endif	/* DASHVIEWITEMDELEGATE_H */

