/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * 
 * Authors:
 *  Alexis López Zubieta
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

#include "DashFactory.h"

Dash* DashFactory::m_dash = NULL;
MenuCache* DashFactory::m_menuCache = NULL;
MenuCacheNotifyId DashFactory::m_menuCacheNotifyId = NULL;

DashFactory::DashFactory() {
    menu_cache_init(SHOW_IN_LXDE | SHOW_IN_KDE);
}

DashFactory::~DashFactory() {
    if (m_dash != NULL) {
        delete(m_dash);
    }
    if (m_menuCache != NULL) {
        menu_cache_remove_reload_notify(m_menuCache, m_menuCacheNotifyId);
        menu_cache_unref(m_menuCache);
    }
}

QWidget* DashFactory::getLauncher(const QString &menuFileName) {
    if (m_menuFileName != menuFileName || m_dash == NULL) {
        //        qDebug() << "Generating menu from: " << menuFileName;

        m_menuFileName = menuFileName;
        m_menuCache = menu_cache_lookup(m_menuFileName.toLocal8Bit());
        m_menuCacheNotifyId = menu_cache_add_reload_notify(m_menuCache, (MenuCacheReloadNotify) updateMenu, m_dash);

        m_dash = new Dash(m_menuCache);
//        m_menu = new XdgCachedMenu(m_menuCache, NULL);
    }
    return m_dash;
}

void DashFactory::updateMenu(MenuCache* cache, gpointer user_data) {
    if (user_data != NULL && cache != NULL) {
//        XdgCachedMenu* menu = reinterpret_cast<XdgCachedMenu*> (user_data);
//
//        menu->rebuildMenu(cache);
    }

}