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

#define QT_NO_KEYWORDS

#include "DashViewModel.h"
#include "GDesktopFile.h"

#include <QUrl>
#include <QList>
#include <QString>
#include <QMenu>
#include <QStringListModel>
#include <QMimeData>
#include <QDebug>

DashViewModel::DashViewModel(const QList<GDesktopFile*>& appList,QObject* parent) : QAbstractListModel(parent) {
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
    appList_.clear();
}

//Qt::DropActions DashViewModel::supportedDropActions() const {
//    return Qt::CopyAction;
//}

Qt::ItemFlags DashViewModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    if (index.isValid()) {
        return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | defaultFlags;
    } else
        return defaultFlags;
}

QMimeData *DashViewModel::mimeData(const QModelIndexList &indexes) const {
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for (QModelIndex index : indexes) {
        if (index.isValid()) {
            QList<QUrl> urls;
            urls.append(QUrl(appList_.at(index.row())->getFilename()));
            qDebug() << "Drag mime data with url" << appList_.at(index.row())->getFilename();
            mimeData->setUrls(urls);
        }
    }
    qDebug() << "Black magic happens to be here";
    mimeData->setData("application/url", encodedData);
    return mimeData;
}

DashViewModel::~DashViewModel() {
    //no idea if I have to free the list here. qDeleteAll is raising an exception
}

