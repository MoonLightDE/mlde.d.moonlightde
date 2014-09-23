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

#ifndef DASHFACTORY_H
#define	DASHFACTORY_H

#include "Dash.h"
#include "panel/IPanel.h"
#include "xdgcachedmenu.h"

class DashFactory : public ILauncherFactory {
public:
    DashFactory();
    virtual ~DashFactory();

    QWidget* getLauncher(const QString &menuFileName);
    static void updateMenu(MenuCache* cache, gpointer user_data);
private:
    static Dash* m_dash;
    QString m_menuFileName;
    static MenuCache* m_menuCache;
    static MenuCacheNotifyId m_menuCacheNotifyId;
};

#endif	/* DASHFACTORY_H */

