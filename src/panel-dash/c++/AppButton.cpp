/* 
 * File:   AppButton.cpp
 * Author: sigfried
 * 
 * Created on September 12, 2014, 2:21 PM
 */

#include "AppButton.h"

#include <QString>
#include <QIcon>
#include <qt5xdg/xdgicon.h>
#include <qt5xdg/xdgdesktopfile.h>

AppButton::AppButton(MenuCacheItem* item, QWidget* parent) : QPushButton(parent), _item(menu_cache_item_ref(item)) {
//    Si es una app le pongo el tooltip
    if (menu_cache_item_get_type(item) == MENU_CACHE_TYPE_APP) {
        QString comment = QString::fromUtf8(menu_cache_item_get_comment(item));
        setToolTip(comment);
    }

    QIcon icon = XdgIcon::fromTheme(menu_cache_item_get_icon(item));
    setIcon(icon);
    connect(this, SIGNAL(released()), SLOT(onClick()));
}

AppButton::~AppButton() {
    if (_item) {
        menu_cache_item_unref(_item);
    }
}

void AppButton::onClick() {
    XdgDesktopFile df;
    char* desktop_file = menu_cache_item_get_file_path(_item);
    df.load(desktop_file);
    g_free(desktop_file);
    df.startDetached();
}

