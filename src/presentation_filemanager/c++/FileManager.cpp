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


#include "ui_filemanager.h"
#include "IconDelegate.h"
#include "model_filesystem/FileSystem.h"
#include "FileManager.h"

#include <usModuleContext.h>
#include <usGetModuleContext.h>

#include <QUrl>
#include <QMenu>
#include <QTimer>
#include <QDebug>
#include <QStandardPaths>
#include <QStackedWidget>
#include <QLineEdit>

FileManager::FileManager(QWidget *parent) :
QWidget(parent),
ui(new Ui::FileManager),
m_BrowsingInstance() {
    ui->setupUi(this);
    setWindowTitle("Filesystem Manager");
    //setWindowIcon(QIcon(":/images/app-icon.png"));
    startPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    ui->lineEdit->setText(startPath);

    ui->lineEdit->setStyleSheet("QLineEdit{background:white;"
            " border: 1px solid lightgrey;"
            " border-right: none;}"
            );
    ui->pushButton->setStyleSheet("QPushButton{background:white;"
            " border: 1px solid lightgrey;"
            " border-left: none;"
            "}");


    modelList = new FileSystemModel();
    modelList->setDirectory(m_BrowsingInstance.currentDir());
    //modelList->setColumnsToView(4); //standard is 5
    modelList->setShowHidden(false); //standard is false


    //connect(modelList,SIGNAL(directoryLoaded(QString)),this,SLOT(HideBusyDialog(QString)));

    bookmarksList = new QListView();
    widget = new QStackedWidget;

    InitIconsView();
    InitDetailsView();

    SetButtonIcons();
    CreateMenus();

    connect(ui->pushButtonBack, SIGNAL(clicked()), this, SLOT(pushButtonBack_clicked()));
    connect(ui->pushButtonNext, SIGNAL(clicked()), this, SLOT(pushButtonNext_clicked()));

    split = new QSplitter(Qt::Horizontal);

    int s1 = (int) (split->width()*0.2);
    int s2 = (int) (split->width() - s1);

    QList<int> sizes;
    sizes << s1 << s2;


    widget->addWidget(tree);
    widget->addWidget(list);

    split->insertWidget(0, bookmarksList);
    split->addWidget(widget);

    split->setSizes(sizes);
    split->setStyleSheet("QSplitter::handle:vertical {height: 3px;}");


    ui->verticalLayout->addWidget(split);
    split->setFocus();
}

FileManager::~FileManager() {
    delete list;
    delete tree;
    delete bookmarksList;
    delete widget;
}

void FileManager::CreateMenus() {

    QMenu *menu_principal = new QMenu();
    QAction* a;

    //menu file
    QMenu* menu = new QMenu();
    menu = menu_principal->addMenu(tr("&File"));
    a = menu->addAction(tr("&New File"));
    a->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
    //connect
    a = menu->addAction(tr("New &Folder"));
    a->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F));
    //connect
    a = menu->addAction(tr("action3"));
    //connect


    //menu edit
    menu = menu_principal->addMenu(tr("&Edit"));
    a = menu->addAction(tr("Copy"));
    a->setShortcut(QKeySequence::Copy);
    //connect
    a = menu->addAction(tr("Cut"));
    a->setShortcut(QKeySequence::Cut);
    //connect
    a = menu->addAction(tr("Paste"));
    a->setShortcut(QKeySequence::Paste);
    //connect


    menu_principal->addSeparator();
    a = menu_principal->addAction(tr("&Settings..."));
    //connect(a,SIGNAL(triggered()),this,SLOT(launchSettings()));
    menu_principal->addSeparator();
    a = menu_principal->addAction(tr("&Close"));
    connect(a, SIGNAL(triggered()), this, SLOT(close()));

    ui->pushButton_options->setMenu(menu_principal);

}

void FileManager::SetButtonIcons() {

}

void FileManager::on_pushButton_details_clicked() {
    if (!tree->isVisible()) {
        widget->setCurrentWidget(tree);
        modelList->setDirectory(m_BrowsingInstance.currentDir());
    }
}

void FileManager::on_pushButton_icons_clicked() {
    if (!list->isVisible()) {
        widget->setCurrentWidget(list);
        modelList->setDirectory(m_BrowsingInstance.currentDir());
    }
}

void FileManager::InitDetailsView() {

    tree = new QTreeView();
    tree->setWordWrap(true);
    tree->setItemsExpandable(false);
    tree->setExpandsOnDoubleClick(false);
    tree->setRootIsDecorated(false);
    tree->setMouseTracking(false);
    tree->setStyleSheet("QTreeView::item:hover {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1.5, stop: 0 #e7effd, stop: 1 #cbdaf1);}"
            "QTreeView::item:selected {border: 1px solid #567dbc;}QTreeView::item:selected:active{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc);}"
            "QTreeView::item:selected:!active {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6b9be8, stop: 1 #577fbf);}");

    tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tree->setEditTriggers(QAbstractItemView::EditKeyPressed |
            QAbstractItemView::SelectedClicked);
    tree->setModel(modelList);
    connect(tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(DetailsItemDoubleClicked(QModelIndex)));

}

void FileManager::InitIconsView() {

    list = new QListView();
    list->setViewMode(QListView::IconMode);
    list->uniformItemSizes();
    list->setResizeMode(QListView::Adjust);
    list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    list->setMovement(QListView::Static);
    list->setSelectionRectVisible(true);
    list->setFocus();
    list->setEditTriggers(QAbstractItemView::EditKeyPressed |
            QAbstractItemView::SelectedClicked);
    list->setWrapping(true);
    list->setItemDelegate(new ItemDelegate(list));
    list->setModel(modelList);

    connect(list, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ListItemDoubleClicked(QModelIndex)));

}

void FileManager::ListItemDoubleClicked(QModelIndex current) {
    modelList->setDirectory(m_BrowsingInstance.currentDir());
    ui->lineEdit->setText(m_BrowsingInstance.currentPath());
}

void FileManager::DetailsItemDoubleClicked(QModelIndex current) {
    modelList->setDirectory(m_BrowsingInstance.currentDir());
    ui->lineEdit->setText(m_BrowsingInstance.currentPath());

}

void FileManager::HideBusyDialog(QString path) {


}

void FileManager::pushButtonBack_clicked() {
    modelList->setDirectory(m_BrowsingInstance.goBack());

}

void FileManager::pushButtonNext_clicked() {
    modelList->setDirectory(m_BrowsingInstance.goForward());
}

void FileManager::pathEditActivated() {
    QString newPath = ui->lineEdit->text();

}

void FileManager::changePlace(const QString &newPath) {
    modelList->setDirectory(m_BrowsingInstance.goTo(newPath));
}