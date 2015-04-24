/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *  Jorge Damian Diaz Morejon
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

#include "FileSystemModel.h"
#include "model_filesystem/Directory.h"
#include "module_config.h"

#include <QDateTime>
#include <QString>
#include <QModelIndex>
#include <QFileInfo>
#include <QIcon>

#include <functional>

QString formatSize(qint64 num) {
    QString total;
    const qint64 kb = 1024;
    const qint64 mb = 1024 * kb;
    const qint64 gb = 1024 * mb;
    const qint64 tb = 1024 * gb;

    if (num >= tb) total = QString("%1TB").arg(QString::number(qreal(num) / tb, 'f', 2));
    else if (num >= gb) total = QString("%1GB").arg(QString::number(qreal(num) / gb, 'f', 2));
    else if (num >= mb) total = QString("%1MB").arg(QString::number(qreal(num) / mb, 'f', 1));
    else if (num >= kb) total = QString("%1KB").arg(QString::number(qreal(num) / kb, 'f', 1));
    else total = QString("%1 bytes").arg(num);

    return total;
}

FileSystemModel::FileSystemModel(QObject *parent)
: QAbstractItemModel(parent) {

    columnsToView = 5;
    showHiddens = false;
}

FileSystemModel::~FileSystemModel() {
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const {
    if (mRootItem == NULL)
        return QVariant();

    QString child = mChilds.at(index.row());

    // Alignment of filename
    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 1) {
            return Qt::AlignVCenter + Qt::AlignLeft;
        }
    }// Display information about file
    else if (role == Qt::DisplayRole) {
        QVariant data;
        if (index.column() <= columnsToView) {
            switch (index.column()) {
                case 0:
                    data = child;
                    break;
                case 1:
                {
                    if (mRootItem->mimetype(child) == "inode/directory")
                        data = "";
                    else
                        data = formatSize(mRootItem->size(child, false));
                    break;
                }
                case 2:
                    data = mRootItem->mimetype(child);
                    break;
                case 3:
                {
                    QDateTime t = QDateTime::fromTime_t((uint) mRootItem->timeModified(child));
                    QString time;
                    time.append(t.date().toString(Qt::SystemLocaleShortDate));
                    time.append(" ");
                    time.append(t.time().toString(Qt::SystemLocaleShortDate));
                    data = time;
                    break;
                }
                case 4:
                {
                    QFlags<QFile::Permissions> perms = mRootItem->permission(child);
                    QString str;
                    str.append(perms.testFlag(QFile::ReadOwner) ? "r" : "-");
                    str.append(perms.testFlag(QFile::WriteOwner) ? "w" : "-");
                    str.append(perms.testFlag(QFile::ExeOwner) ? "x" : "-");
                    str.append(" ");
                    str.append(perms.testFlag(QFile::ReadGroup) ? "r" : "-");
                    str.append(perms.testFlag(QFile::WriteGroup) ? "w" : "-");
                    str.append(perms.testFlag(QFile::ExeGroup) ? "x" : "-");
                    str.append(" ");
                    str.append(perms.testFlag(QFile::ReadOther) ? "r" : "-");
                    str.append(perms.testFlag(QFile::WriteOther) ? "w" : "-");
                    str.append(perms.testFlag(QFile::ExeOther) ? "x" : "-");
                    data = str;
                    break;
                }
                default:
                    data = "";
                    break;
            }

        }
        return data;
    }// Display file icon
    else if (role == Qt::DecorationRole) {
        if (index.column() != 0)
            return QVariant();
        QString name = mRootItem->iconName(child);
        return QIcon::fromTheme(name, QIcon());

    }// Display file name
    else if (role == Qt::EditRole) {
        return child;
    }

    if (role == Qt::StatusTipRole) {
        return child;
    }

    return QVariant();
}

QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const {

    if (role == Qt::DisplayRole)
        switch (section) {
            case 0: return tr("Name");
            case 1: return tr("Size");
            case 2: return tr("Type");
            case 3: return tr("Date Modified");
            case 4: return tr("Permissions");
            default: return QVariant();
        }

    return QVariant();

}

Qt::ItemFlags FileSystemModel::flags(const QModelIndex &index) const {

    if (!index.isValid()) return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable |
            Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

}

int FileSystemModel::rowCount(const QModelIndex &parent) const {
    if (mRootItem == NULL)
        return 0;
    return mChilds.count();
}

QModelIndex FileSystemModel::index(int row, int column, const QModelIndex &parent) const {

    return createIndex(row, column);
}

QModelIndex FileSystemModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}

int FileSystemModel::columnCount(const QModelIndex &parent) const {
    if (mRootItem == NULL)
        return 0;

    return columnsToView;
}



//TODO: implement this method for get mime types
// from files

QStringList FileSystemModel::mimeTypes() const {

}

//TODO: implement this method for insert
// new rows to the model

bool FileSystemModel::insertRows(int row, int count, const QModelIndex &parent) {

}

//TODO: implement this methods for insert new
// file or foldes to the file system

QModelIndex FileSystemModel::insertFile(QModelIndex parent) {

}

QModelIndex FileSystemModel::insertFolder(QModelIndex parent) {

}

QString FileSystemModel::fileName(const QModelIndex &index) {
    if (mRootItem == NULL)
        return QString();

    return mChilds.at(index.row());
}

QString FileSystemModel::filePath(const QModelIndex &index) {
    if (mRootItem == NULL)
        return QString();

    QString name = fileName(index);
    return mRootItem->childUri(name);
}

void FileSystemModel::setDirectory(model_filesystem::Directory * directory) {
    mRootItem = directory;
    connect(dynamic_cast<QObject*> (directory), SIGNAL(changed()), this, SLOT(setupModel()));
    setupModel();
}

QString FileSystemModel::path() const {
    if (mRootItem == NULL)
        return QString();

    return mRootItem->uri();
}

void FileSystemModel::setShowHidden(bool hide) {
    showHiddens = hide;
    setupModel();
}

void FileSystemModel::setColumnsToView(short columns) {
    if (columns > 1) {
        columnsToView = columns;
        setupModel();
    }

}

void FileSystemModel::setupModel() {
    beginResetModel();
    mChilds.clear();
    if (mRootItem != NULL)
        sortChilds(mRootItem->children(), showHiddens);
    endResetModel();
}

void FileSystemModel::sortChilds(QStringList list, bool showHiddens) {
    QStringList files, direct;
    QString uri;
    if (showHiddens) {
        for (int i = 0; i < list.size(); i++) {
            uri = mRootItem->childUri(list.at(i));
            if (mRootItem->mimetype(list.at(i)) == "inode/directory")
                direct.append(list.at(i));
            else
                files.append(list.at(i));
        }
    } else {
        for (int i = 0; i < list.size(); i++) {
            if (list.at(i).at(0) != '.') {
                uri = mRootItem->childUri(list.at(i));
                if (mRootItem->mimetype(list.at(i)) == "inode/directory")
                    direct.append(list.at(i));
                else
                    files.append(list.at(i));
            }
        }
    }

    direct.sort(Qt::CaseInsensitive);
    files.sort(Qt::CaseInsensitive);
    mChilds << direct << files;
}



