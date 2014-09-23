/*
 * File:   Dash.cpp
 * Author: alexis
 *
 * Created on 10 de septiembre de 2014, 16:21
 */

#include "Dash.h"

#include "AppButton.h"
//#include "xdgcachedmenu.h"

#include <menu-cache/menu-cache.h>

#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QLabel>

Dash::Dash(MenuCache* menuCache) {
    widget.setupUi(this);
    setWindowFlags(Qt::Popup);
    setFrameStyle(QFrame::NoFrame);

    build(menuCache);
}

Dash::~Dash() {
}

void Dash::build(MenuCache* menuCache) {
    QGridLayout *layout = new QGridLayout(widget.tabApps);


    const QSize iconSize(64, 64);
    const QSize boxSize(64, 64);
    //    const int maxRow = (qApp->desktop()->screenGeometry().height() * 0.8) / boxSize;
    const int maxColumn = (qApp->desktop()->screenGeometry().width() * 0.5) / boxSize.width();
    qDebug() << "maxColumn: " << maxColumn;
    int currentRow = 0;
    int currentColumn = 0;

    GSList* appList = menu_cache_list_all_apps(menuCache);
    for (GSList* itr = appList; itr; itr = itr->next) {
        MenuCacheItem *item = (MenuCacheItem *) itr->data;
        MenuCacheType type = menu_cache_item_get_type(item);

        if (type == MENU_CACHE_TYPE_APP) {
//            XdgCachedMenuAction* action = new XdgCachedMenuAction(item, widget.tabApps);
            AppButton * bttn = new AppButton((MenuCacheItem *) item, this); 
            //            QPushButton *bttn = new QPushButton(widget.tabApps);
            QString name = menu_cache_item_get_name(item);
            QLabel *label = new QLabel(name);
            
            label->setWordWrap(TRUE);
            label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            label->setMaximumSize(boxSize);
            
            bttn->setFlat(true);
            bttn->setIconSize(iconSize);
            bttn->setMinimumSize(boxSize);
            bttn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

            layout->addWidget(bttn, currentRow, currentColumn, Qt::AlignHCenter);
            layout->addWidget(label, currentRow + 1, currentColumn, Qt::AlignHCenter);

            currentColumn++;
            if (currentColumn >= maxColumn) {
                currentColumn = 0;
                currentRow += 2;
            }
            //            currentRow++;
            //            if (currentRow >= maxRow) {
            //                currentRow = 0;
            //                currentColumn++;
            //            }
        }
    }
    g_slist_free_full(appList, (void (*)(void*)) menu_cache_item_unref);


    widget.apps->setGeometry(QRect(0, 0, maxColumn * boxSize.width(), currentRow * boxSize.height()));
    widget.apps->setLayout(layout);
}

void Dash::free() {
    
}

void Dash::addMenuItems(QMenu* menu, MenuCacheDir* dir) {
    //        GSList* l;
    //    for (l = menu_cache_dir_list_children(dir); l; l = l->next) {
    //        MenuCacheItem* item = (MenuCacheItem*) l->data;
    //        MenuCacheType type = menu_cache_item_get_type(item);
    //
    //        if (type == MENU_CACHE_TYPE_SEP) {
    //            menu->addSeparator();
    //            continue;
    //        } else {
    //            XdgCachedMenuAction* action = new XdgCachedMenuAction(item, menu);
    //            menu->addAction(action);
    //            if (type == MENU_CACHE_TYPE_APP)
    //                connect(action, SIGNAL(triggered(bool)), SLOT(onItemTrigerred()));
    //            else if (type == MENU_CACHE_TYPE_DIR) {
    //                XdgCachedMenu* submenu = new XdgCachedMenu(menu);
    //                action->setMenu(submenu);
    //                addMenuItems(submenu, (MenuCacheDir*) item);
    //            }
    //        }
    //    }
    //
    //    g_slist_free_full(l, (void (*)(void*)) menu_cache_item_unref);
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
