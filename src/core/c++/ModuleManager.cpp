/*
 * Copyright (C) 2013 Moonlight Desktop Environment Team
 * Authors:
 * Alexis López Zubieta
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

#include "ModuleManager.h"
#include "core/IModuleManager.h"
#include "MoonLightDECoreConfig.h"

#include <usModule.h>
#include <usModuleContext.h>
#include <usModuleRegistry.h>
#include <usSharedLibrary.h>
#include <usServiceProperties.h>

#include <set>

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QApplication>

US_USE_NAMESPACE

ModuleManager::ModuleManager(const QString &profile, const QString &pluginsDir) :
m_pluginsDirSTD(pluginsDir.toStdString()), m_pluginsDir(pluginsDir) {

    qDebug() << "NOTICE: Using prifile " << profile;

    QSettings settings(QApplication::applicationName(), profile);
    if (!QFile::exists(settings.fileName())) {
        qDebug() << "WARNING: The specified profile doesn't exist:\n\t" <<
                settings.fileName() <<
                "\n\tLoading core configuration UI.";
        //showCoreSettingsManager(profile);
        return;
    }

    qDebug() << "[    ] Loading registered modules" << endl;
    int nmodules = settings.beginReadArray("Modules");
    if (nmodules > 0) {
        for (int i = 0; i < nmodules; i++) {
            settings.setArrayIndex(i);
            QString moduleName = settings.value("Name").toString();
            loadModule(moduleName);
        }
        settings.endArray();
    } else {
        qDebug() << "ERROR: Sorry there are no configured modules at " << settings.fileName() << endl;
    }
    qDebug() << "[DONE] Loading registered modules" << endl;

}

ModuleManager::~ModuleManager() {
}

void ModuleManager::loadModule(const QString &name, const bool &persistent) {
    qDebug() << "Loading " << name;
    SharedLibrary sharedLib(m_pluginsDirSTD, "");
    Module* module = ModuleRegistry::GetModule(name.toStdString());
    if (!module) {
        try {
            std::map<std::string, SharedLibrary>::iterator libIter =
                    libraryHandles.find(sharedLib.GetFilePath(name.toStdString()));
            if (libIter != libraryHandles.end()) {
                libIter->second.Load();
            } else {
                QDir dir(m_pluginsDir);
                if (!dir.exists("lib" + name + ".so")) {
                    qDebug() << "ERROR: Unknown module: " << name << ".\n"
                            << "\tPlease check that is installed in this path: " << m_pluginsDir;
                } else {
                    SharedLibrary libHandle(m_pluginsDirSTD, name.toStdString());
                    libHandle.Load();
                    libraryHandles.insert(std::make_pair(libHandle.GetFilePath(), libHandle));
                }
            }

        } catch (const std::exception& e) {
            qDebug() << e.what() << endl;
        }
    } else if (!module->IsLoaded()) {
        try {
            const std::string modulePath = module->GetLocation();
            std::map<std::string, SharedLibrary>::iterator libIter =
                    libraryHandles.find(modulePath);
            if (libIter != libraryHandles.end()) {
                libIter->second.Load();
            } else {
                SharedLibrary libHandle(m_pluginsDirSTD, name.toStdString());
                libHandle.Load();
                libraryHandles.insert(std::make_pair(libHandle.GetFilePath(), libHandle));
            }
        } catch (const std::exception& e) {
            qDebug() << e.what() << endl;
        }
    } else if (module) {
        qDebug() << "NOTICE: module " << name << " already loaded" << endl;
    }
}

void ModuleManager::unloadModule(const QString &name, const bool &persistent) {
    Module * const module = ModuleRegistry::GetModule(name.toStdString());
    if (module) {
        std::map<std::string, SharedLibrary>::iterator libIter =
                libraryHandles.find(module->GetLocation());
        if (libIter == libraryHandles.end()) {
            std::cout << "Info: Unloading not possible. The module was loaded by a dependent module." << std::endl;
        } else {
            try {
                libIter->second.Unload();

                // Check if it has really been unloaded
                if (module->IsLoaded()) {
                    std::cout << "Info: The module is still referenced by another loaded module. It will be unloaded when all dependent modules are unloaded." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
}
