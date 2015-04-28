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


#include "DashViewModel.h"

#include <QList>
#include <QString>
#include <QMenu>


DashViewModel::DashViewModel(const QList<GDesktopFile*>& appList, QObject* parent) : QAbstractListModel(parent) {
    appList_ = appList;
}

int DashViewModel::rowCount(const QModelIndex &parent) const {
    return appList_.count();
}

QVariant DashViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return QVariant();
    if (index.row() >= appList_.count()) return QVariant();

    if (role == Qt::DisplayRole) {
        // Only returns something for the roles you support (DisplayRole is a minimum)
        // Here we assume that the "Employee" class has a "lastName" method but of course any string can be returned
        return QVariant(appList_.at(index.row())->getName());
    } else if (role == Qt::DecorationRole) {
        return QVariant(appList_.at(index.row())->getIcon());
    } else {
        return QVariant();
    }
}

GDesktopFile* DashViewModel::getDesktop(int rowIndex) {
    return appList_.at(rowIndex);
}

void DashViewModel::clear(void) {
    while (!appList_.empty()) {
        appList_.removeFirst();
    }
}

DashViewModel::~DashViewModel() {
}

