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
#ifndef DASHVIEWMODEL_H
#define	DASHVIEWMODEL_H
#include <QAbstractListModel>
#include <qt5xdg/XdgDesktopFile>
#include <QList>

class DashViewModel : public QAbstractListModel  {
    Q_OBJECT
    
public:
    explicit DashViewModel(const QList<XdgDesktopFile*>& appList,QObject* parent = 0);
//    DashViewModel(const DashViewModel& orig);
    virtual ~DashViewModel();
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    XdgDesktopFile* getDesktop(int rowIndex);
    void clear();
    
//    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    
private:
    QList<XdgDesktopFile*> appList_;
};

#endif	/* DASHVIEWMODEL_H */

