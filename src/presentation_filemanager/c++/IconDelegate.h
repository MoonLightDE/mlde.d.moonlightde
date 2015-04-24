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

#ifndef ICONDELEGATE_H
#define ICONDELEGATE_H

#include<QAbstractItemDelegate>

class ItemDelegate : public QAbstractItemDelegate
{
public:

    ItemDelegate(QObject *parent);
    ItemDelegate(const ItemDelegate& orig);
    virtual ~ItemDelegate();
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index )const;
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index )const;

    private:

};
#endif // ICONDELEGATE_H
