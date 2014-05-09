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
//#include "core/IModuleManager.h"
#include "MoonLightDECoreConfig.h"

#include <usModule.h>
#include <usSharedLibrary.h>
#include <usModuleContext.h>
#include <usModuleRegistry.h>
#include <usGetModuleContext.h>
#include <usServiceProperties.h>

#include <set>

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QApplication>

US_USE_NAMESPACE

ModuleManager::ModuleManager(const QString &aditionalLibsPath) {
    paths.append(MODULES_OUTPUT_DIR);
    paths.append(aditionalLibsPath);
}

ModuleManager::~ModuleManager() {
}

bool ModuleManager::load(const QString &name) {
    QString err_msg;

    Module* module = ModuleRegistry::GetModule(name.toStdString());
    if (module) { // Check if the module was loaded previously
        if (module->IsLoaded()) {
            qDebug() << "Module " << name << " already loaded.";
        } else { // Reload module
            try {
                SharedLibrary *libHandle;
                if (libs.contains(name)) {
                    libHandle = libs.value(name);
                } else {
                    QString modulePath = QString::fromStdString(module->GetLocation());
                    libHandle = new SharedLibrary(modulePath.toStdString());
                }

                libHandle->Load();
                libs.insert(name, libHandle);

                qDebug() << "Module loaded: " << name;
                return true;
            } catch (const std::exception& e) {
                err_msg.append("\n\t");
                err_msg.append(e.what());
            }
        }
    } else { // Load module from scratch

        foreach(QString path, paths) {
            try {
                SharedLibrary *libHandle = new SharedLibrary(path.toStdString(), name.toStdString());
                libHandle->Load();

                libs.insert(name, libHandle);
                qDebug() << "Module loaded: " << name;
                return true;
            } catch (const std::exception& e) {
                err_msg.append("\n\t");
                err_msg.append(e.what());
            }
        }

    }
    qWarning() << "Unable to load module: " << name << err_msg;
    return false;
}

bool ModuleManager::unload(const QString &name) {
    QString err_msg;

    Module * const module = ModuleRegistry::GetModule(name.toStdString());
    if (module) {
        try {
            SharedLibrary *libHandle;
            if (libs.contains(name)) {
                libHandle = libs.value(name);
            } else {
                libHandle = new SharedLibrary(module->GetLocation());
            }

            libHandle->Unload();

            // Check if it has really been unloaded
            if (module->IsLoaded()) {
                qDebug() << "Fail to unload: " << name;
                qDebug() << "Info: The module is still referenced by another loaded module. It will be unloaded when all dependent modules are unloaded.";

                return false;
            } else {
                qDebug() << "Module unloaded: " << name;
                return true;
            }
        } catch (const std::exception& e) {
            qDebug() << "Fail to unload: " << name;
            qDebug() << "\n\t" << e.what();
        }
    }
    qDebug() << "Module " << name << " wasn't loaded, nothing to do.";
    return true;
}

void ModuleManager::loadFromProfile(QSettings * profile) {
    qDebug() << "Loading list: " << profile->fileName();

    int nmodules = profile->beginReadArray("Modules");
    if (nmodules > 0) {
        for (int i = 0; i < nmodules; i++) {
            profile->setArrayIndex(i);
            QString moduleName = profile->value("Name").toString();
            load(moduleName);
        }
        profile->endArray();
    } else {
        qWarning() << "Sorry, there are no modules names.";
    }
    qDebug() << "Loading list finished.";
}

QList<QString> ModuleManager::listAviableModules() {
    QList<QString> list;
    QStringList nameFilters;
    nameFilters << "*.so";

    foreach(QString path, paths) {
        QDir dir(path);
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
        dir.setNameFilters(nameFilters);
        QStringList filesList = dir.entryList();

        foreach(QString fileName, filesList) {
            fileName.chop(3);
            fileName.remove(0, 3);
            list.append(fileName);
        }
    }
    return list;
}

QList<QString> ModuleManager::listActiveModules() {
    QList<QString> list;

    foreach(Module * module, ModuleRegistry::GetLoadedModules()) {
        list.append(QString::fromStdString(module->GetName()));
    }
    return list;
}
