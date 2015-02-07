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

#include "ModuleManager.h"
#include "MoonLightDECoreConfig.h"

#include <usModule.h>
#include <usSharedLibrary.h>
#include <usModuleContext.h>
#include <usModuleRegistry.h>
#include <usGetModuleContext.h>
#include <usServiceProperties.h>

#include <set>

#include <QDir>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QApplication>

US_USE_NAMESPACE

ModuleManager::ModuleManager(const QHash<QString, QVariant> config) {
    qApp->addLibraryPath(MODULES_INSTALL_DIR);
    qApp->addLibraryPath(config["modulesPath"].toString());

    m_descriptorsPaths.append(DESCRIPTORS_INSTALL_DIR);
    m_descriptorsPaths.append(config["descriptorsPath"].toString());
    qDebug() << "Module manager configuration:";
    qDebug() << "\tModules paths: " << qApp->libraryPaths();
    qDebug() << "\tDescriptors paths: " << m_descriptorsPaths;

    m_context = us::GetModuleContext();

    m_CoreSettingsTracker = new us::ServiceTracker<QSettings>(
            m_context, us::LDAPFilter(std::string("(&(") + us::ServiceConstants::OBJECTCLASS() + "=" +
            us_service_interface_iid<QSettings>() + ")" + "(Module=Core))")
            );
    m_CoreSettingsTracker->Open();
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
            SharedLibrary *libHandle;
            try {    
                if (libs.contains(name)) {
                    libHandle = libs.value(name);
                } else {
                    QString modulePath = QString::fromStdString(module->GetLocation());
                    libHandle = new SharedLibrary();
                    libHandle->SetPrefix(LIBS_PREFIX);
                    libHandle->SetFilePath(modulePath.toStdString());
                }

                libHandle->Load();
                libs.insert(name, libHandle);

                qDebug() << "Module loaded: " << name;
                return true;
            } catch (const std::exception& ex) {
                err_msg.append("\n\t");
                err_msg.append(ex.what());
                
                delete libHandle;
            }
        }
    } else { // Load module from scratch

        foreach(QString path, qApp->libraryPaths()) {
            SharedLibrary *libHandle;
            try {
                libHandle = new SharedLibrary();
                libHandle->SetPrefix(LIBS_PREFIX);
                libHandle->SetLibraryPath(path.toStdString());
                libHandle->SetName(name.toStdString());

                libHandle->Load();

                libs.insert(name, libHandle);
                qDebug() << "Module loaded: " << name;
                return true;
            } catch (const std::exception& ex) {
                err_msg.append("\n\t");
                err_msg.append(ex.what());
                
                delete libHandle;
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
        SharedLibrary *libHandle;
        try {
            if (libs.contains(name)) {
                libHandle = libs.value(name);
                libs.remove(name);
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
        
        delete libHandle;
    }
    qDebug() << "Module " << name << " wasn't loaded, nothing to do.";
    return true;
}

QList<QString> ModuleManager::getAviableModules() {
    QList<QString> list;
    QStringList nameFilters;
    nameFilters << LIBS_PREFIX"*.so";

    foreach(QString path, qApp->libraryPaths()) {
        QDir dir(path);
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
        dir.setNameFilters(nameFilters);
        QFileInfoList filesList = dir.entryInfoList();

        foreach(QFileInfo fileInfo, filesList) {
            QString descriptorFile = fileInfo.baseName();
            descriptorFile.remove(LIBS_PREFIX);
            list.append(descriptorFile);
        }
    }
    return list;
}

QList<QString> ModuleManager::getActiveModules() {
    QList<QString> list;
    foreach(Module * module, ModuleRegistry::GetLoadedModules()) {
        list.append(QString::fromStdString(module->GetName()));
    }
    return list;
}

XdgDesktopFile * ModuleManager::getModuleDescriptor(const QString moduleName) {
    XdgDesktopFile * descriptor = new XdgDesktopFile();

    QString fullname = moduleName + ".desktop";
    descriptor->setValue("Name", moduleName);
    descriptor->setValue("Comment", "No descriptor provided.");

    foreach(QString path, m_descriptorsPaths) {
        QDir dir(path);
        if (dir.exists(fullname)) {
            descriptor->load(dir.filePath(fullname));
            break;
        }
    }
    return descriptor;
}

const QStringList ModuleManager::getStartUpModules() const {
    QStringList list;
    QSettings * settings = m_CoreSettingsTracker->GetService();
    if (settings == NULL)
        qDebug() << "Unable to locate the CoreSettings Service.";
    else {
        qDebug() << "Reading startup modules list from: " << settings->fileName();

        int nmodules = settings->beginReadArray("Modules");
        if (nmodules > 0) {
            for (int i = 0; i < nmodules; i++) {
                settings->setArrayIndex(i);
                QString moduleName = settings->value("Name").toString();
                list.append(moduleName);
            }
        } else {
            qWarning() << "There are any configured modules to load.";
        }
        settings->endArray();
    }


    return list;
}

void ModuleManager::setStartUpModules(const QStringList &modules) {
    QSettings * settings = m_CoreSettingsTracker->GetService();
    if (settings == NULL)
        qDebug() << "Unable to locate the CoreSettings Service.";
    else {
        settings->beginWriteArray("Modules", modules.size());
        int pos = 0;

        foreach(QString moduleName, modules) {
            settings->setArrayIndex(pos);
            settings->setValue("Name", moduleName);
            pos++;
        }
        settings->endArray();
    }
}