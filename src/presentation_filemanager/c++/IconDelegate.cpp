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

ItemDelegate::ItemDelegate(QObject*parent):QAbstractItemDelegate(parent) {
}

ItemDelegate::~ItemDelegate() {
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)
const{
    QSize s_retorno(180,180);
     return s_retorno;
}


void ItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{

    painter->setFont(QFont("Arial",10));

    if(option.state & QStyle::State_HasFocus ){
         QString txt = index.data(Qt::DisplayRole).toString();


      painter->fillRect(option.rect, Qt::gray);
       QRect r = option.rect;

        r.moveTo(QPoint(r.x(),r.y()+55));
      r.setSize(QSize(r.width()-20,r.height()-20));
      painter->drawText(r, Qt::AlignHCenter|Qt::TextWrapAnywhere,txt);


      QIcon ic = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QRect r_general=option.rect;

    r_general.setHeight(option.rect.height()+20);
     QRect r_icon = r_general;
     r_icon.moveTo(QPoint(r_icon.x()+50,r_icon.y()));
     r_icon.setSize(QSize(60,60));
      ic.paint(painter, r_icon, Qt::AlignVCenter|Qt::AlignLeft);


    }
    else{

        QIcon ic = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QString txt = index.data(Qt::DisplayRole).toString();
        QRect r_general=option.rect;
        r_general.setHeight(option.rect.height()+20);
        QRect r_icon = r_general;
        r_icon.moveTo(QPoint(r_icon.x()+50,r_icon.y()));
        r_icon.setSize(QSize(60,60));
        ic.paint(painter, r_icon, Qt::AlignVCenter|Qt::AlignHCenter);

        QRect r = r_general;
        r.moveTo(QPoint(r.x()+20,r.y()+60));
        r.setSize(QSize(r.width()-50,r.height()));

        QTextLayout textLayout(txt);
        textLayout.setFont(option.font);
        int widthUsed = 0;
        int lineCont = 0;
        textLayout.beginLayout();

        while (++lineCont < 3) {
            QTextLine line = textLayout.createLine();
            if (!line.isValid())
                break;

            line.setLineWidth(option.rect.height());
            widthUsed += line.naturalTextWidth();

        }
        textLayout.endLayout();


        widthUsed += option.rect.width();

        QString newText = painter->fontMetrics().elidedText(txt, Qt::ElideRight, widthUsed);

        painter->drawText(r,  Qt::AlignHCenter|Qt::TextWrapAnywhere , newText);
    }



    }
