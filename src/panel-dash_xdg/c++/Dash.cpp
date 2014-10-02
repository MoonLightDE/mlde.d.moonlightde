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

#include <algorithm>


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
    m_ui.setupUi(this);
    setWindowFlags(Qt::Popup);
    setFrameStyle(QFrame::NoFrame);
    built = false;
}

Dash::~Dash() {
}

void Dash::build() {
    QTime time;
    time.start();

    const QRect screen = qApp->desktop()->screenGeometry();

    const QSize iconSize(64, 64);
    const QSize boxSize(100, 100);
    const int maxColumnApps = m_ui.tabs->size().width() / (boxSize.width() + 32);
    
    GridLayoutVExpanding* layoutApps = new GridLayoutVExpanding(maxColumnApps, m_ui.tabApps);
    
    layoutApps->setSpacing(0);
    layoutApps->setMargin(16);
    
    GridLayoutVExpanding* layoutSettings = new GridLayoutVExpanding(maxColumnApps, m_ui.tabSettings);
    
    layoutSettings->setSpacing(0);
    layoutSettings->setMargin(16);
    
    qDebug() << "Layouts init: " << time.elapsed();
    // Individual items are released inside the AppButton class
    QList<XdgDesktopFile*> appList = XdgDesktopFileCache::getAllFiles();
    std::sort(appList.begin(), appList.end(), XdgDesktopFileComparisonFunctor());
    //    qSort(appList.begin(), appList.end());
    qDebug() << "XdgDesktopFile lockup: " << time.elapsed();

    foreach(XdgDesktopFile * app, appList) {
        if (app->type() != XdgDesktopFile::ApplicationType) {
            delete app;
            continue;
        }
        AppButton *bttn = new AppButton(app, this);

        QLabel *label = new QLabel(app->name());
        label->setAlignment(Qt::AlignHCenter);

        label->setWordWrap(true);
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        label->setMaximumSize(boxSize.width(), 35);
        
        bttn->setFlat(true);
        bttn->setIconSize(iconSize);
        bttn->setMinimumSize(boxSize);
        bttn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

        connect(bttn, &AppButton::released, this, &Dash::hide);
        
        //Layout for widget
        QVBoxLayout* vBoxLayout = new QVBoxLayout();            
        vBoxLayout->addWidget(bttn);
        vBoxLayout->addWidget(label);
        
        QString appCategories = app->value("Categories", "None").toString();
        
        if (appCategories.contains("Settings", Qt::CaseInsensitive)) {
            
            layoutSettings->addLayout(vBoxLayout);
        } else {
            
            layoutApps->addLayout(vBoxLayout);
        }

    }
    qDebug() << "Layouts built: " << time.elapsed();

    qDebug() << "appcontainerSize" << m_ui.tabs->size();

    m_ui.apps->setGeometry(0, 0, m_ui.scrollArea->size().width() - (layoutApps->spacing() * maxColumnApps) 
                         , layoutApps->getCurrentRow() * boxSize.height());
    m_ui.apps->setLayout(layoutApps);


    m_ui.settings->setGeometry(0, 0, m_ui.scrollAreaSettings->size().width()
                             , layoutApps->getCurrentRow() * boxSize.height());
    m_ui.settings->setLayout(layoutSettings);

}

void Dash::free() {

}

void Dash::onItemTrigerred() {
    //    XdgCachedMenuAction* action = static_cast<XdgCachedMenuAction*> (sender());
    //    XdgDesktopFile df;
    //    char* desktop_file = menu_cache_item_get_file_path(action->item());
    //    df.load(desktop_file);
    //    g_free(desktop_file);
    //    df.startDetached();
}

// taken from libqtxdg: XdgMenuWidget



// taken from libqtxdg: XdgMenuWidget

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
    }

    m_ui.tabs->setCurrentWidget(m_ui.tabStart);
    show();
}
