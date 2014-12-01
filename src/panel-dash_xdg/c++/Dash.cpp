/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 *      Jorge Fernández Sánchez
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

#include "Dash.h"
#include "AppButton.h"
#include "GridLayoutHExpanding.h"
#include "GridLayoutVExpanding.h"
#include "DesktopFileCollection.h"

#include "qevent.h"
#include "qdir.h"
#include "core/ModuleSettings.h"
#include "DashViewModel.h"
#include "DashViewItemDelegate.h"


#include <qt5xdg/XdgDesktopFile>

#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QApplication>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QRect>
#include <QLabel>
#include <QTime>
#include <QTextStream>
#include <QListView>
#include <QMenu>

#include <algorithm>

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <QThread>

#include <usGetModuleContext.h>
#include <qt5xdg/XdgIcon>



QTextStream cout(stdout);

Dash::Dash() : m_settings("panel-dash_xdg") {
    startDashModel = NULL;
    appDashModel = NULL;
    settingsDashModel = NULL;

    m_ui.setupUi(this);

    setWindowFlags(Qt::Popup);
    setFrameStyle(QFrame::NoFrame);
    built = false;

    monitor = new QFileSystemWatcher();
    monitor->addPath("/usr/share/applications");
    connect(m_ui.lineEdit, SIGNAL(textChanged(QString)), SLOT(searchEditChanged(QString)));
    connect(m_ui.lineEdit, SIGNAL(returnPressed()), SLOT(onReturnPressed()));
    connect(monitor, SIGNAL(directoryChanged(QString)), SLOT(onApplicationsFolderChanged()));
    appListGenerator = new DesktopFileCollection();
    getFavorites();
    appIndex = -1;
    //    EditEventFilter* editeventfilter = new EditEventFilter();
    //    m_ui.lineEdit->installEventFilter(editeventfilter);
    m_ui.lineEdit->installEventFilter(this);
    m_ui.tabs->installEventFilter(this);
    m_ui.StartView->installEventFilter(this);
    m_ui.AppView->installEventFilter(this);
    m_ui.SettingsView->installEventFilter(this);
    XdgIcon::setThemeName("FaenzaFlattr");
}

Dash::~Dash() {
}

void Dash::configView(QListView* view) {
    view->setItemDelegate(new DashViewItemDelegate());
    view->setViewMode(QListView::IconMode);
    view->setSpacing(26);
    view->setIconSize(QSize(48, 48));

    // TODO: Implement an item delegate to give an apropiated size to the
    //  items.
    view->setUniformItemSizes(true);
    view->setWrapping(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectItems);

    view->setTextElideMode(Qt::ElideLeft);

    view->setWordWrap(true);

    view->setMovement(QListView::Static);
    view->setResizeMode(QListView::Adjust);

    view->setLayoutMode(QListView::Batched);
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    //volver a calcular esto
    if (screenGeometry.width() >= 1360) {
        view->setBatchSize(84); //para 1366X768
    } else {
        view->setBatchSize(56); //para 1024x768
    }

    view->setContextMenuPolicy(Qt::CustomContextMenu);

}

void Dash::build() {
    configView(m_ui.AppView);
    configView(m_ui.SettingsView);
    configView(m_ui.StartView);

    QList<XdgDesktopFile*> appList = appListGenerator->all();

    appDashModel = new DashViewModel(appList);
    m_ui.AppView->setModel(appDashModel);

    QHash<QString, QString>* filters = new QHash<QString, QString>();
    filters->insert(QString("Categories"), "Settings");
    QList<XdgDesktopFile*> settingsList = appListGenerator->filter(*filters);

    settingsDashModel = new DashViewModel(settingsList);
    m_ui.SettingsView->setModel(settingsDashModel);

    connect(m_ui.AppView, SIGNAL(activated(const QModelIndex&)), SLOT(onItemTrigerred(const QModelIndex&)));
    connect(m_ui.SettingsView, SIGNAL(activated(const QModelIndex&)), SLOT(onItemTrigerred(const QModelIndex&)));
    connect(m_ui.StartView, SIGNAL(activated(const QModelIndex&)), SLOT(onItemTrigerred(const QModelIndex&)));

    connect(m_ui.AppView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showContextMenuForApp(QPoint)));

    connect(m_ui.SettingsView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showContextMenuForApp(QPoint)));

    connect(m_ui.StartView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showContextMenuForStart(QPoint)));

}

bool Dash::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_ui.lineEdit) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *> (event);
            if (keyEvent->key() == Qt::Key_Down) {
                if (m_ui.tabs->currentIndex() == 0) {
                    m_ui.StartView->setFocus();
                    m_ui.StartView->setCurrentIndex(startDashModel->index(0, 0));
                } else if (m_ui.tabs->currentIndex() == 1) {
                    m_ui.AppView->setFocus();
                    m_ui.AppView->setCurrentIndex(appDashModel->index(0, 0));
                } else if (m_ui.tabs->currentIndex() == 2) {
                    m_ui.SettingsView->setFocus();
                    m_ui.SettingsView->setCurrentIndex(settingsDashModel->index(0, 0));
                }

                qDebug() << "Down key pressed";
                return true;
            } else {
                return QObject::eventFilter(obj, event);
            }

        } else {
            // standard event processing
            return QObject::eventFilter(obj, event);
        }
    } else {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *> (event);
            if (keyEvent->key() != Qt::Key_Down
                    && keyEvent->key() != Qt::Key_Up
                    && keyEvent->key() != Qt::Key_Right
                    && keyEvent->key() != Qt::Key_Left
                    && keyEvent->key() != Qt::Key_Enter) {
                m_ui.lineEdit->setFocus();
                m_ui.lineEdit->setText(keyEvent->text());
                return QObject::eventFilter(obj, event);
            }
        }
        return QObject::eventFilter(obj, event);
    }

}

void Dash::onReturnPressed() {
    qDebug() << "Return pressed";
    if (m_ui.lineEdit->text().length() > 0 && startDashModel->rowCount() > 0) {
        startDashModel->getDesktop(0)->startDetached();
        m_ui.StartView->clearSelection();
        getFavorites();
    }
}

void Dash::showContextMenuForApp(QPoint pos) {
    qDebug() << "showContextMenuForApp " << pos.x() << " " << pos.y();
    if (m_ui.tabs->currentIndex() == 1) {
        if (m_ui.AppView->selectionModel()->selectedIndexes().length() >= 1) {
            appIndex = m_ui.AppView->selectionModel()->selectedIndexes().at(0).row();
        }
    } else {
        if (m_ui.SettingsView->selectionModel()->selectedIndexes().length() >= 1) {
            appIndex = m_ui.SettingsView->selectionModel()->selectedIndexes().at(0).row();
        }
    }
    qDebug() << appIndex;

    if (appIndex >= 0) {
        QMenu* contextMenu = new QMenu();
        QAction* addAction = new QAction(tr("Add to favorites"), this);
        connect(addAction, SIGNAL(triggered()), SLOT(addFavorite()));
        contextMenu->addAction(addAction);
        contextMenu->exec(mapToGlobal(pos));
    }
}

void Dash::showContextMenuForStart(QPoint pos) {
    qDebug() << "showContextMenuForApp " << pos.x() << " " << pos.y();
    //    appIndex = m_ui.StartView->indexAt(mapFromGlobal(pos)).row() - 1;
    if (m_ui.StartView->selectionModel()->selectedIndexes().length() >= 1) {
        appIndex = m_ui.StartView->selectionModel()->selectedIndexes().at(0).row();
    }
    qDebug() << appIndex;

    if (appIndex >= 0) {
        QMenu* contextMenu = new QMenu();
        QAction* addAction;
        if (m_ui.lineEdit->text().length() == 0) {
            addAction = new QAction(tr("Remove from favorites"), this);
            connect(addAction, SIGNAL(triggered()), SLOT(removeFavorite()));
        } else {
            addAction = new QAction(tr("Add to favorites"), this);
            connect(addAction, SIGNAL(triggered()), SLOT(addFavorite()));
        }
        contextMenu->addAction(addAction);
        contextMenu->exec(mapToGlobal(pos));
    }
}

void Dash::addFavorite() {
    if (appIndex >= 0) {
        XdgDesktopFile* theApp;
        if (m_ui.tabs->currentIndex() == 1) {
            theApp = appDashModel->getDesktop(appIndex);
        } else if (m_ui.tabs->currentIndex() == 2) {
            theApp = settingsDashModel->getDesktop(appIndex);
        } else {
            theApp = startDashModel->getDesktop(appIndex);
        }
        qDebug() << "app" << appIndex << " -->> " << theApp->name();
        addFavorites(theApp);
        getFavorites();
        appIndex = -1;
    }
}

void Dash::removeFavorite() {
    if (appIndex >= 0) {
        XdgDesktopFile* theApp = startDashModel->getDesktop(appIndex);
        qDebug() << "app" << appIndex << " -->> " << theApp->name();
        removeFavorites(theApp);
        getFavorites();
        appIndex = -1;
    }
}

void Dash::cleanApps() {
    if (appDashModel != NULL) {
        appDashModel->clear();
    }

    if (settingsDashModel != NULL) {
        settingsDashModel->clear();
    }
}

void Dash::buildSearch(QString search) {
    QList<XdgDesktopFile*> startList;
    if (search.length() == 0) {
        startList = QList<XdgDesktopFile*>();
    } else {
        QHash<QString, QString>* filters = new QHash<QString, QString>();
        filters->insert(QString("Name"), search);
        filters->insert(QString("Categories"), search);
        filters->insert(QString("Comment"), search);
        startList = appListGenerator->filter(*filters);
    }

    startDashModel = new DashViewModel(startList);
    delete m_ui.StartView->model();
    m_ui.StartView->setModel(startDashModel);
}

void Dash::onItemTrigerred(const QModelIndex& item) {
    DashViewModel* themodel;
    QListView *theview;

    if (m_ui.tabs->currentIndex() == 0) {
        qDebug() << "StartItemTrigerred" << item.row();
        themodel = startDashModel;
        theview = m_ui.StartView;
    } else if (m_ui.tabs->currentIndex() == 1) {
        qDebug() << "AppItemTrigerred" << item.row();
        themodel = appDashModel;
        theview = m_ui.AppView;
    } else if (m_ui.tabs->currentIndex() == 2) {
        qDebug() << "SettingsItemTrigerred" << item.row();
        themodel = settingsDashModel;
        theview = m_ui.SettingsView;
    }


    qDebug() << themodel->getDesktop(item.row())->name();
    themodel->getDesktop(item.row())->startDetached();
    theview->clearSelection();

    if (m_ui.tabs->currentIndex() == 0) {
        getFavorites();
    }

    hide();
}

void Dash::addFavorites(XdgDesktopFile* app) {
    us::ModuleContext* context = us::GetModuleContext();
    const QString ruta(ModuleSettings::getModuleDataLocation(context));

    const QString appName(app->fileName());
    QDir* dir = new QDir(ruta);
    dir->mkdir("favs");
    QFile* file = new QFile(appName);
    if (file->copy(dir->absolutePath() + "/favs/" + app->name().toLower())) {
        qDebug() << "Archivo copiado a favoritos";
    } else {
        qDebug() << "Error copiando archivo a favoritos";
    }
}

//Get all favorite apps in the directory and paint them on start widget

void Dash::getFavorites() {
    m_ui.lineEdit->clear();
    us::ModuleContext* context = us::GetModuleContext();
    const QString ruta(ModuleSettings::getModuleDataLocation(context) + "/favs");

    qDebug() << "Ruta: " << endl;
    qDebug() << ruta;
    QDir* favsDir = new QDir(ruta);

    QFileInfoList list = favsDir->entryInfoList(QDir::Files, QDir::Name);
    QList<XdgDesktopFile*> favAppList;

    if (!list.empty()) {

        foreach(QFileInfo app, list) {
            const QString cadena(app.filePath());
            //You have to specify the absolute path to the file, otherwise it wont work
            XdgDesktopFile* fav = XdgDesktopFileCache::getFile(cadena);
            favAppList.append(fav);
        }
    }
    //Call method putFavorites
    putFavorites(favAppList);
}

//jfsanchez@estudiantes.uci.cu

void Dash::hideEvent(QHideEvent *event) {
    qDebug() << "hideEvent()";
    QThread::msleep(1);
    QFrame::hideEvent(event);
}

void Dash::putFavorites(QList<XdgDesktopFile*> favAppList) {

    startDashModel = new DashViewModel(favAppList);
    delete m_ui.StartView->model();
    m_ui.StartView->setModel(startDashModel);
}

//TODO: Dinamically update the start widget showing|unshowing the favorites

void Dash::removeFavorites(XdgDesktopFile* app) {
    qDebug() << "Certainly YOU SHALL NOT PASS!!!!";
    us::ModuleContext* context = us::GetModuleContext();
    const QString ruta(ModuleSettings::getModuleDataLocation(context) + "/favs");

    QDir* favsDir = new QDir(ruta);

    QFileInfoList list = favsDir->entryInfoList(QDir::Files, QDir::Name);

    foreach(QFileInfo file, list) {
        //        qDebug() << file.fileName();
        //        qDebug() << app->name();
        if (file.fileName() == app->name().toLower()) {
            qDebug() << "You should buy a pet :P";
            const QString ruta(app->fileName());
            QFile* archive = new QFile(ruta);
            archive->remove();
        }
    }
}

void Dash::showEvent(QShowEvent * event) {
    qDebug() << "Show event";

    if (!built) {
        build();
        built = true;
    } else {
        //no recuerdo pa q era esto
        //            cleanApps();
        //            build();
        getFavorites();
    }

    m_ui.tabs->setCurrentWidget(m_ui.tabStart);
    m_ui.lineEdit->setFocus();

    show();
}

void Dash::onApplicationsFolderChanged() {
    qDebug() << "Applications folder changed";
    cleanApps();
    build();
}

void Dash::searchEditChanged(QString asearch) {
    qDebug() << "search edit changed " << asearch;
    if (asearch.length() > 0) {
        qDebug() << "searching for " << asearch;
        m_ui.tabs->setCurrentIndex(0);
        buildSearch(asearch);

        m_ui.StartView->setCurrentIndex(startDashModel->index(0, 0));
    } else {
        getFavorites();
    }
}
