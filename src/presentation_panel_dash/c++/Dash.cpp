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

#define QT_NO_KEYWORDS

#include "GDesktopFile.h"
#include "GDesktopFileCollection.h"
#include "DashViewItemDelegate.h"

#include "module_config.h"
#include "Dash.h"
//#include "AppButton.h"
#include "core/ModuleSettings.h"

#include <qt5xdg/XdgDesktopFile>
#include <qt5xdg/XdgIcon>

#include <usGetModuleContext.h>

#include <KWindowSystem>

#include <QDebug>
#include <QDir>
#include <QKeyEvent>
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
#include <QThread>

#include <algorithm>

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <qt5/QtCore/qlogging.h>




//de abajo , m_settings("panel-dash_xdg")
Dash::Dash(QWidget * parent) : QDialog(parent) {
    startDashModel = NULL;
    appDashModel = NULL;
    settingsDashModel = NULL;

    m_ui.setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::Popup | Qt::X11BypassWindowManagerHint);

    built = false;

    monitor = new QFileSystemWatcher();
    monitor->addPath("/usr/share/applications");
    connect(m_ui.lineEdit,  SIGNAL(textChanged(QString)), SLOT(searchEditChanged(QString)));
    connect(m_ui.lineEdit, SIGNAL(returnPressed()), SLOT(onReturnPressed()));
    connect(monitor, SIGNAL(directoryChanged(QString)), SLOT(onApplicationsFolderChanged()));
    appListGenerator = new GDesktopFileCollection();
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
    delete monitor;
    delete appDashModel;
    delete settingsDashModel;
    delete startDashModel;
    delete appListGenerator;
}

void Dash::configView(QListView* view) {
    view->setItemDelegate(new DashViewItemDelegate());
    view->setViewMode(QListView::IconMode);
    view->setSpacing(26);
    view->setIconSize(QSize(48, 48));

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
    //TODO: Improve items bach size
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

    QList<GDesktopFile*> appList = appListGenerator->getAll();

    appDashModel = new DashViewModel(appList);
    m_ui.AppView->setModel(appDashModel);

    QHash<QString, QString>* filters = new QHash<QString, QString>();
    filters->insert(QString("category"), "Settings");
    QList<GDesktopFile*> settingsList = appListGenerator->filter(*filters);

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

                // qDebug() << MODULE_NAME_STR <<  "Down key pressed";
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
            if (keyEvent->key() == Qt::Key_Return) {
                // qDebug() << MODULE_NAME_STR << "Enter key pressed";
                return QObject::eventFilter(obj, event);
            } else if (keyEvent->key() != Qt::Key_Down
                    && keyEvent->key() != Qt::Key_Up
                    && keyEvent->key() != Qt::Key_Right
                    && keyEvent->key() != Qt::Key_Left
                    && keyEvent->key() != Qt::Key_Return) {
                m_ui.lineEdit->setFocus();
                m_ui.lineEdit->setText(keyEvent->text());
                return QObject::eventFilter(obj, event);
            }
        } else {
            return QObject::eventFilter(obj, event);
        }
    }

}

void Dash::onReturnPressed() {
    //qDebug() <<  MODULE_NAME_STR << "Return key pressed";
    if (m_ui.lineEdit->text().length() > 0 && startDashModel->rowCount() > 0) {
        startDashModel->getDesktop(0)->launchApp();
        m_ui.StartView->clearSelection();
        getFavorites();
        hide();
    }
}

void Dash::showContextMenuForApp(QPoint pos) {
    // qDebug() <<  MODULE_NAME_STR << "showContextMenuForApp " << pos.x() << " " << pos.y();
    if (m_ui.tabs->currentIndex() == 1) {
        if (m_ui.AppView->selectionModel()->selectedIndexes().length() >= 1) {
            appIndex = m_ui.AppView->selectionModel()->selectedIndexes().at(0).row();
        }
    } else {
        if (m_ui.SettingsView->selectionModel()->selectedIndexes().length() >= 1) {
            appIndex = m_ui.SettingsView->selectionModel()->selectedIndexes().at(0).row();
        }
    }
    // qDebug() << MODULE_NAME_STR << appIndex;

    if (appIndex >= 0) {
        QMenu* contextMenu = new QMenu();
        QAction* addAction = new QAction(tr("Add to favorites"), this);
        connect(addAction, SIGNAL(triggered()), SLOT(addFavorite()));
        contextMenu->addAction(addAction);
        contextMenu->exec(mapToGlobal(pos));
    }
}

void Dash::showContextMenuForStart(QPoint pos) {
    //    qDebug() <<  MODULE_NAME_STR << MODULE_NAME_STR << "showContextMenuForApp " << pos.x() << " " << pos.y();
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
        GDesktopFile* theApp;
        if (m_ui.tabs->currentIndex() == 1) {
            theApp = appDashModel->getDesktop(appIndex);
        } else if (m_ui.tabs->currentIndex() == 2) {
            theApp = settingsDashModel->getDesktop(appIndex);
        } else {
            theApp = startDashModel->getDesktop(appIndex);
        }
        //        qDebug() <<  MODULE_NAME_STR << "app" << appIndex << " -->> " << theApp->name();
        addFavorites(theApp);
        getFavorites();
        appIndex = -1;
    }
}

void Dash::removeFavorite() {
    if (appIndex >= 0) {
        GDesktopFile* theApp = startDashModel->getDesktop(appIndex);
        //        qDebug() <<  MODULE_NAME_STR <<  "app" << appIndex << " -->> " << theApp->name();
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
    QList<GDesktopFile*> startList;
    if (search.length() == 0) {
        startList = QList<GDesktopFile*>();
    } else {
        QHash<QString, QString>* filters = new QHash<QString, QString>();
        filters->insert(QString("name"), search);
        filters->insert(QString("categories"), search);
        filters->insert(QString("description"), search);
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
        //        qDebug() <<  MODULE_NAME_STR << "StartItemTrigerred" << item.row();
        themodel = startDashModel;
        theview = m_ui.StartView;
    } else if (m_ui.tabs->currentIndex() == 1) {
        //        qDebug() <<  MODULE_NAME_STR << "AppItemTrigerred" << item.row();
        themodel = appDashModel;
        theview = m_ui.AppView;
    } else if (m_ui.tabs->currentIndex() == 2) {
        //        qDebug() <<  MODULE_NAME_STR << "SettingsItemTrigerred" << item.row();
        themodel = settingsDashModel;
        theview = m_ui.SettingsView;
    }


    //    qDebug() <<  MODULE_NAME_STR << themodel->getDesktop(item.row())->name();
    themodel->getDesktop(item.row())->launchApp();
    theview->clearSelection();

    if (m_ui.tabs->currentIndex() == 0) {
        getFavorites();
    }

    hide();
}

void Dash::addFavorites(GDesktopFile* app) {
    us::ModuleContext* context = us::GetModuleContext();
    const QString ruta(ModuleSettings::getModuleDataLocation(context));

    const QString appName(app->getFilename());
    QDir* dir = new QDir(ruta);
    dir->mkdir("favs");
    QFile* file = new QFile(appName);
    bool result = file->copy(dir->absolutePath() + "/favs/" + app->getName().toLower());
    //    if (result) {
    //        qDebug() <<  MODULE_NAME_STR << "File copy success";
    //    } else {
    //        qDebug() <<  MODULE_NAME_STR << "Error copiando archivo a favoritos";
    //    }
}

//Get all favorite apps in the directory and paint them on start widget

void Dash::getFavorites() {
    m_ui.lineEdit->clear();
    us::ModuleContext* context = us::GetModuleContext();
    const QString ruta(ModuleSettings::getModuleDataLocation(context) + "/favs");

    //    qDebug() <<  MODULE_NAME_STR << "Ruta: " << endl;
    //    qDebug() <<  MODULE_NAME_STR << ruta;
    QDir* favsDir = new QDir(ruta);

    QFileInfoList list = favsDir->entryInfoList(QDir::Files, QDir::Name);
    QList<GDesktopFile*> favAppList;

    if (!list.empty()) {
        for(QFileInfo app :list) {
            const QString path(app.filePath());
            //You have to specify the absolute path to the file, otherwise it wont work
            //qDebug() << "fav for "<< path.toUtf8().constData();
            GDesktopFile* fav = new GDesktopFile(g_desktop_app_info_new_from_filename(path.toUtf8().constData()));
            favAppList.append(fav);
        }
    }
    putFavorites(favAppList);
}

/**
 * Hide event, blame jfsanchez@estudiantes.uci.cu for this.
 * @param event
 */
void Dash::hideEvent(QHideEvent * event) {
    //    qDebug() <<  MODULE_NAME_STR << "hideEvent()";
    QDialog::hideEvent(event);
    QApplication::setActiveWindow(parentWidget());
}

void Dash::putFavorites(QList<GDesktopFile*> favAppList) {
    startDashModel = new DashViewModel(favAppList);
    delete m_ui.StartView->model();
    m_ui.StartView->setModel(startDashModel);
}

//TODO: Dinamically update the start widget showing|unshowing the favorites

void Dash::removeFavorites(GDesktopFile * app) {
    //    qDebug() <<  MODULE_NAME_STR << "Certainly YOU SHALL NOT PASS!!!!"; // What the hell!! Is this a debug message!!??
    us::ModuleContext* context = us::GetModuleContext();
    const QString ruta(ModuleSettings::getModuleDataLocation(context) + "/favs");

    QDir* favsDir = new QDir(ruta);

    QFileInfoList list = favsDir->entryInfoList(QDir::Files, QDir::Name);

    for(QFileInfo file : list) {
        //        qDebug() << file.fileName();
        //        qDebug() << app->name();
        
        if (file.fileName().compare(app->getFilename(),Qt::CaseInsensitive)) {
            //            qDebug() <<  MODULE_NAME_STR  << "You should buy a pet :P"; // Again ? what the hell with you !?
            const QString ruta(app->getFilename());
            QFile* archive = new QFile(ruta);
            archive->remove();
        }
    }
}

void Dash::showEvent(QShowEvent * event) {
    //    qDebug() << MODULE_NAME_STR << "Show event";
    if (!built) {
        build();
        built = true;
    } else {
        //no recuerdo pa q era esto
        //            cleanApps();
        //            build();
        getFavorites();
    }

    // Current monitor screen
    QRect geometry = QApplication::desktop()->availableGeometry(this);
    // Whole virtual screen
    // QRect geometry = KWindowSystem::workArea();
    this->setGeometry(geometry);


    m_ui.tabs->setCurrentWidget(m_ui.tabStart);
    m_ui.lineEdit->setFocus();

    QApplication::setActiveWindow(this);
    show();
}

void Dash::onApplicationsFolderChanged() {
    //    qDebug() <<  MODULE_NAME_STR << "Applications folder changed";
    cleanApps();
    build();
}

void Dash::searchEditChanged(QString asearch) {
    //    qDebug() <<  MODULE_NAME_STR  << "search edit changed " << asearch;
    if (asearch.length() > 0) {
        //        qDebug() <<  MODULE_NAME_STR << "searching for " << asearch;
        m_ui.tabs->setCurrentIndex(0);
        buildSearch(asearch);

        m_ui.StartView->setCurrentIndex(startDashModel->index(0, 0));
    } else {
        getFavorites();
    }
}

void Dash::toggle() {
    if (isVisible())
        hide();
    else
        show();
}
