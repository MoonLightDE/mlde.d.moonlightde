/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
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

#include "module_config.h"

#include "NodeGVFS.h"
#include "FileSystemGVFS.h"

#include <QApplication>
#include <QDebug>

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

US_USE_NAMESPACE
/**
 */
class Activator : public ModuleActivator {
private:

    /**
     * Implements ModuleActivator::Load().
     *
     * @param context the framework context for the module.
     */
    void Load(ModuleContext* context) {
        //        context->RegisterService<model_filesystem::FileSystem>(&m_FS, ServiceProperties());
        runTests("file:///home");
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
    }

    void runTests(QString path) {
        qDebug() << MODULE_NAME_STR << " : Running tests";
        GVFSDirectory * dir = m_FS.getDirectory(path);
        // Test dir caching
        GVFSDirectory * dir2 = m_FS.getDirectory(path);
        m_FS.releaseDirectory(dir2);
        
        qDebug() << MODULE_NAME_STR << " : dir name" << dir->name();
        qDebug() << MODULE_NAME_STR << " : dir size" << dir->size(".", true);
        
        qDebug() << MODULE_NAME_STR << " : dir parent" << dir->parentUri();
        qDebug() << MODULE_NAME_STR << " : dir children" << dir->childern();
        for (QString childName: dir->childern()){
            qDebug() << MODULE_NAME_STR << " : " << childName << " type: " << dir->mimetype(childName);
            if (dir->mimetype(childName) == "inode/directory") {
                GVFSDirectory * childDir = m_FS.getDirectory(dir->childUri(childName));
                qDebug() << MODULE_NAME_STR << " : " << childDir->uri() << " childs " << childDir->childern();
                m_FS.releaseDirectory(childDir);
            }
        }
        m_FS.releaseDirectory(dir);
        qDebug() << MODULE_NAME_STR << " : Tests finished";
    }

    FileSystemGVFS m_FS;
};
US_EXPORT_MODULE_ACTIVATOR(model_filesystem_gvfs, Activator)
