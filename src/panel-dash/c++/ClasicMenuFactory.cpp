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

#include "ClasicMenuFactory.h"
#include <QDebug>

MenuCache* ClasicMenuFactory::m_menuCache = NULL;
XdgCachedMenu* ClasicMenuFactory::m_menu = NULL;
MenuCacheNotifyId ClasicMenuFactory::m_menuCacheNotifyId = NULL;

ClasicMenuFactory::ClasicMenuFactory() : ILauncherFactory() {
    menu_cache_init(SHOW_IN_LXDE | SHOW_IN_KDE);
}

ClasicMenuFactory::~ClasicMenuFactory() {
    if (m_menu != NULL) {
        delete(m_menu);
    }
    if (m_menuCache != NULL) {
        menu_cache_remove_reload_notify(m_menuCache, m_menuCacheNotifyId);
        menu_cache_unref(m_menuCache);
    }
}

QWidget* ClasicMenuFactory::getLauncher(const QString &menuFileName) {
    if (m_menuFileName != menuFileName || m_menu == NULL) {
//        qDebug() << "Generating menu from: " << menuFileName;

        m_menuFileName = menuFileName;
        m_menuCache = menu_cache_lookup(m_menuFileName.toLocal8Bit());
        m_menuCacheNotifyId = menu_cache_add_reload_notify(m_menuCache, (MenuCacheReloadNotify) updateMenu, m_menu);

        m_menu = new XdgCachedMenu(m_menuCache, NULL);
    }
    return m_menu;
}

void ClasicMenuFactory::updateMenu(MenuCache* cache, gpointer user_data) {
    if (user_data != NULL && cache != NULL) {
        XdgCachedMenu* menu = reinterpret_cast<XdgCachedMenu*> (user_data);

        menu->rebuildMenu(cache);
    }
  
}