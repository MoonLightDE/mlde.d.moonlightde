/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
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

#ifndef IPANEL_H
#define	IPANEL_H

#include <QWidget>
#include <QString>
#include <usServiceInterface.h>

struct IPanel {
    virtual ~IPanel(){};
};

struct ILauncherFactory {
    virtual QWidget* getLauncher(const QString &menuFileName) = 0; 
    virtual ~ILauncherFactory(){};
};

struct IQuickLauncher {
    virtual ~IQuickLauncher(){};
};

struct ITaskBar {
    virtual ~ITaskBar(){};
};

struct ISystemTray {
    virtual ~ISystemTray(){};
};

struct IClock {
    virtual ~IClock(){};
};

US_DECLARE_SERVICE_INTERFACE(IPanel, "org.moonlightde.panel.IPanel/1.0")
US_DECLARE_SERVICE_INTERFACE(ILauncherFactory, "org.moonlightde.panel.ILauncher/1.0")
US_DECLARE_SERVICE_INTERFACE(IQuickLauncher, "org.moonlightde.panel.IQuickLauncher/1.0")
US_DECLARE_SERVICE_INTERFACE(ITaskBar, "org.moonlightde.panel.ITaskBar/1.0")
US_DECLARE_SERVICE_INTERFACE(ISystemTray, "org.moonlightde.panel.ISystemTry/1.0")
US_DECLARE_SERVICE_INTERFACE(IClock, "org.moonlightde.panel.IClock/1.0")
#endif	/* IPANEL_H */

