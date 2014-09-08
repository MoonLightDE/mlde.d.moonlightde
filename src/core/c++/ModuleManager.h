/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
 * 
 * This file is part of Moonlight Desktop Environment Core.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODULEMANAGER_H
#define	MODULEMANAGER_H

#include "core/ICore.h"
#include "core/IQt5.h"

#include <qt5xdg/XdgDesktopFile>

#include <usServiceTracker.h>
#include <usModuleContext.h>
#include <usSharedLibrary.h>

#include <QString>
#include <QHash>
#include <QStringList>

#include <string>
#include <algorithm>

US_USE_NAMESPACE


class ModuleManager : public Core::IModuleManager {
public:
    ModuleManager(const QHash<QString, QVariant> config);

    bool load(const QString &name);
    bool unload(const QString &name);
    QList<QString> getAviableModules();
    QList<QString> getActiveModules();

    const QStringList getStartUpModules() const;
    void setStartUpModules(const QStringList &modules);
    
    XdgDesktopFile * getModuleDescriptor(const QString moduleName);

    virtual ~ModuleManager();

private:
    QStringList m_descriptorsPaths;
    QHash<QString, SharedLibrary*> libs;
    
    us::ModuleContext* m_context;
    us::ServiceTracker<QSettings>* m_CoreSettingsTracker;
};

#endif	/* MODULEMANAGER_H */

