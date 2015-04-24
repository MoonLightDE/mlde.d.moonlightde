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

#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QString>

namespace model_filesystem{
    class Directory;
}
class FileSystemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit FileSystemModel(QObject *parent = 0);
    ~FileSystemModel();

    QString path() const;
    void setDirectory(model_filesystem::Directory * directory);

    QModelIndex insertFile(QModelIndex parent);
    QModelIndex insertFolder(QModelIndex parent);

    QString fileName(const QModelIndex& index);
    QString filePath(const QModelIndex& index);

    void loadCacheInfo();
    void showThumbnails(bool);
    void loadMimeTypes();
    void setShowHidden(bool);
    void setColumnsToView(short);

protected:
    /**
     * @brief Returns model data (information about directories and files)
     * @param index
     * @param role
     * @return model data
     */
    QVariant data(const QModelIndex &index, int role) const;


    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex parent(const QModelIndex &child) const;

    bool insertRows(int row, int count, const QModelIndex &parent);

//    QModelIndex index(const QString& childName) const;
    QModelIndex index(int row, int column,
                        const QModelIndex & parent = QModelIndex()) const;

    QStringList mimeTypes() const;
protected Q_SLOTS:
    void setupModel();
private:
    model_filesystem::Directory* mRootItem;
    QStringList mChilds;
    QString mRootPath;
    int columnsToView;
    bool showHiddens;

    
    void sortChilds(QStringList,bool);
};

#endif // FILESYSTEMMODEL_H
