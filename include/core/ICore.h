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

#include <QObject>
#include <QString>
#include <QSettings>
#include <QProcessEnvironment>

namespace CoreContext {

    class ICore : public QObject {
    public:
        virtual void start() = 0;
        virtual void finish() = 0;

    signals:
        void started();
    };

    class IEnvironment : public QObject {
    public:
        virtual QProcessEnvironment generateProcessEnvironment() = 0;
        virtual void insert(QString key, QString value) = 0;
        virtual void remove(QString key) = 0;
        virtual bool contains(QString key) = 0;

    signals:
        virtual void environmentUpdated(QString key, QString value) = 0;

    };
    class IModuleManager {
    public:
        virtual bool load(const QString &name) = 0;
        virtual bool unload(const QString &name) = 0;
        virtual void loadFromProfile(QSettings * profile) = 0;
        virtual QList<QString> listAviableModules() = 0;
        virtual QList<QString> listActiveModules() = 0;

    };

    /*Public interface for the "SettingsProfile" class.*/
    class ISettingsProfile {
    public:
        virtual QSettings * getSettingsOf (QObject * object = 0) = 0;
        virtual QSettings * getSettingsOf (const QString & objectPath) = 0;

    };


}
US_DECLARE_SERVICE_INTERFACE(CoreContext::ICore, "org.moonlightde.core.ICore/1.0")
US_DECLARE_SERVICE_INTERFACE(CoreContext::IEnvironment, "org.moonlightde.core.IEnvironment/1.0")
US_DECLARE_SERVICE_INTERFACE(CoreContext::IModuleManager, "org.moonlightde.core.IModuleManager/1.0")
US_DECLARE_SERVICE_INTERFACE(CoreContext::ISettingsProfile, "org.moonlightde.core.ISettingsProfile/1.0")

#endif	/* ICORE_H */

