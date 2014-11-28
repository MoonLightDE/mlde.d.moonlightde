/*
 * File:   Dash.cpp
 * Author: alexis
 *
 * Created on 10 de septiembre de 2014, 16:21
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

/**
 * As XdgDesktopFile doesn't provides the opperator < we must implement it here.
 */
struct XdgDesktopFileComparisonFunctor {

    bool
    operator()(const XdgDesktopFile * __x, const XdgDesktopFile * __y) const {
        return __x->name() < __y->name();
    }
};

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
    connect(monitor, SIGNAL(directoryChanged(QString)), SLOT(onApplicationsFolderChanged()));
    appListGenerator = new DesktopFileCollection();
    getFavorites();
    appIndex = -1;
    XdgIcon::setThemeName("FaenzaFlattr");
}

Dash::~Dash() {
}

void Dash::configView(QListView* view) {
    view->setItemDelegate(new DashViewItemDelegate());
    view->setViewMode(QListView::IconMode);
    view->setSpacing(20);
    view->setIconSize(QSize(48, 48));

    // TODO: Implement an item delegate to give an apropiated size to the
    //  items.
    view->setUniformItemSizes(true);
    view->setWrapping(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectItems);

    view->setTextElideMode(Qt::ElideMiddle);
 
    view->setWordWrap(true);

    view->setMovement(QListView::Static);
    view->setResizeMode(QListView::Adjust);

    view->setLayoutMode(QListView::Batched);
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    if (screenGeometry.width() >= 1360){
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

    connect(m_ui.AppView, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(onAppItemTrigerred(const QModelIndex&)));
    connect(m_ui.SettingsView, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(onSettingsItemTrigerred(const QModelIndex&)));
    connect(m_ui.StartView, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(onStartItemTrigerred(const QModelIndex&)));
    //    connect(m_ui.AppView, SIGNAL(clicked(const QModelIndex&)), SLOT(onAppItemTrigerred(const QModelIndex&)));
    //    connect(m_ui.SettingsView, SIGNAL(clicked(const QModelIndex&)), SLOT(onSettingsItemTrigerred(const QModelIndex&)));
    //    connect(m_ui.StartView, SIGNAL(clicked(const QModelIndex&)), SLOT(onStartItemTrigerred(const QModelIndex&)));

    connect(m_ui.AppView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showContextMenuForApp(QPoint)));

    connect(m_ui.SettingsView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showContextMenuForApp(QPoint)));

    connect(m_ui.StartView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showContextMenuForStart(QPoint)));

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
        m_ui.lineEdit->setText("");
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

void Dash::onAppItemTrigerred(const QModelIndex& item) {
    qDebug() << "AppItemTrigerred" << item.row();
    qDebug() << appDashModel->getDesktop(item.row())->name();
    appDashModel->getDesktop(item.row())->startDetached();
    m_ui.AppView->clearSelection();
    hide();
}

void Dash::onSettingsItemTrigerred(const QModelIndex& item) {
    qDebug() << "SettingsItemTrigerred" << item.row();
    settingsDashModel->getDesktop(item.row())->startDetached();
    m_ui.SettingsView->clearSelection();
    hide();
}

void Dash::onStartItemTrigerred(const QModelIndex& item) {
    qDebug() << "StartItemTrigerred" << item.row();
    startDashModel->getDesktop(item.row())->startDetached();
    m_ui.StartView->clearSelection();
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
        qDebug() << "Archivo copiado";
    } else {
        qDebug() << "Error";
    }
}

//Get all favorite apps in the directory and paint them on start widget

void Dash::getFavorites() {

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

void Dash::handleMouseMoveEvent(QMouseEvent *event) {
    //    if (!(event->buttons() & Qt::LeftButton))
    //        return;
    //
    //    if ((event->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance())
    //        return;
    //
    //    XdgCachedMenuAction *a = qobject_cast<XdgCachedMenuAction*>(actionAt(event->pos()));
    //    if (!a)
    //        return;
    //
    //    QList<QUrl> urls;
    //    char* desktop_file = menu_cache_item_get_file_path(a->item());
    //    urls << QUrl(desktop_file);
    //    g_free(desktop_file);
    //
    //    QMimeData *mimeData = new QMimeData();
    //    mimeData->setUrls(urls);
    //
    //    QDrag *drag = new QDrag(this);
    //    drag->setMimeData(mimeData);
    //    drag->exec(Qt::CopyAction | Qt::LinkAction);
}

void Dash::showEvent(QShowEvent * event) {
    qDebug() << "Show event";

    if (!built) {
        build();
        built = true;
    } else {
        //no recuerdo pa q era esto
        cleanApps();
        build();
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
    } else {
        getFavorites();
    }
}
