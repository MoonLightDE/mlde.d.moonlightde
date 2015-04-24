/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *      Jorge Damian Diaz Morejon
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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "FileSystemModel.h"
#include "FileSystemsTracker.h"

#include "model_filesystem/FileSystem.h"



#include <QStackedWidget>
#include <QFrame>
#include <QSplitter>
#include <QListView>
#include <QTreeView>

namespace Ui {
    class FileManager;
}

class FileManager : public QWidget {
    Q_OBJECT

public:
    FileManager(QWidget *parent = 0);
    ~FileManager();

    void addFileSystem(model_filesystem::FileSystem * fileSystem);
    void removeFileSystem(model_filesystem::FileSystem* fileSystem);

    private
Q_SLOTS:
    void on_pushButton_details_clicked();

    void on_pushButton_icons_clicked();
    void ListItemDoubleClicked(QModelIndex);
    void DetailsItemDoubleClicked(QModelIndex);
    void HideBusyDialog(QString);

    void pushButtonBack_clicked();

    void pushButtonNext_clicked();



private:

    Ui::FileManager *ui;
    void InitDetailsView();
    void InitIconsView();
    void CreateMenus();
    void SetButtonIcons();
    void LaunchBusyDialog();
    model_filesystem::Directory * getDirectory(QString path);

    int index;
    QListView *list;
    QTreeView *tree;
    QListView *bookmarksList;
    QStackedWidget* widget;
    QSplitter* split;
    QString startPath;
    FileSystemModel *modelList;
    QStringList history;
    QList<model_filesystem::FileSystem* > m_AvailableFileSystems;
    
    FileSystemsTracker m_Tracker;
};


#endif // FILEMANAGER_H
