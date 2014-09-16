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

#ifndef ICORE_H
#define	ICORE_H
#include <usServiceInterface.h>

#include <qt5xdg/XdgDesktopFile>

#include <QObject>
#include <QString>
#include <QSettings>
#include <QProcessEnvironment>

namespace Core {

    class IController : public QObject {
    public:
        virtual void start() = 0;
        virtual void finish() = 0;

signals:
        void started();
        void finishing();
    };

    class IModuleManager {
    public:
        virtual bool load(const QString &name) = 0;
        virtual bool unload(const QString &name) = 0;

        virtual QList<QString> getAviableModules() = 0;
        virtual QList<QString> getActiveModules() = 0;
        virtual XdgDesktopFile * getModuleDescriptor(const QString moduleName) = 0;

        virtual const QStringList getStartUpModules() const = 0;
        virtual void setStartUpModules(const QStringList &modules) = 0;
    };

}

Q_DECLARE_INTERFACE(Core::IController, "org.moonlightde.core.IController/1.0")
US_DECLARE_SERVICE_INTERFACE(Core::IController, "org.moonlightde.core.IController/1.0")

US_DECLARE_SERVICE_INTERFACE(Core::IModuleManager, "org.moonlightde.core.IModuleManager/1.1")

#endif	/* ICORE_H */

