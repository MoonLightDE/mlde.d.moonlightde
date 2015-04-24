/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
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

#include "module_config.h"

#include "FileSystemGVFS.h"
#include "GVFSVolumeManager.h"

#include "model_filesystem/VolumeManager.h"
#include "model_filesystem/FileSystem.h"

#include <QObject>
#include <QDebug>
#include <QFuture>
#include <QFutureWatcher>


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
        context->RegisterService<model_filesystem::FileSystem>(&m_FS, ServiceProperties());
        context->RegisterService<model_filesystem::VolumeManager>(&m_VolumeManager, ServiceProperties());
        runTests("fiel:///home/alexis");
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
        model_filesystem::Directory * dir = m_FS.getDirectory(path);

        QFutureWatcher<void> * dirWatcher = new QFutureWatcher<void> ();
        dirWatcher->setFuture(dir->status());

//        QObject::connect(dir, &Directory::failure, [] (QString msg) {
//            qDebug() << MODULE_NAME_STR << " dir lookup error: " << msg;
//        });

        QObject::connect(dirWatcher, &QFutureWatcher<void>::finished, [this, dirWatcher, dir] () {
            qDebug() << MODULE_NAME_STR << " dir lookup finished" << dirWatcher->progressText();

            qDebug() << MODULE_NAME_STR << " : dir name" << dir->name();
                    //        qDebug() << MODULE_NAME_STR << " : dir size" << dir->size(".", true);
                    qDebug() << MODULE_NAME_STR << " : dir parent" << dir->parentUri();


            for (QString childName : dir->children()) {
                qDebug() << MODULE_NAME_STR << "file: " << childName;

                        qDebug() << MODULE_NAME_STR << "\tMIMETYPE: " << childName << " type: " << dir->mimetype(childName);
                        //                        qDebug() << MODULE_NAME_STR << "\tSIZE: " << dir->size(childName, true);
                        //                        qDebug() << MODULE_NAME_STR << "\tSIZE STORED: " << dir->storedSize(childName, true);
                        qDebug() << MODULE_NAME_STR << "\tTIME SINCE FILE WAS MODIFIED: " << dir->timeModified(childName);
                        qDebug() << MODULE_NAME_STR << "\tTIME SINCE FILE WAS ACCESSED: " << dir->timeAccess(childName);
                        qDebug() << MODULE_NAME_STR << "\tTIME SINCE FILE WAS CHANGED: " << dir->timeChanged(childName);
                        qDebug() << MODULE_NAME_STR << "\tOWNER USER: " << dir->ownerUser(childName);
                        qDebug() << MODULE_NAME_STR << "\tOWNER GROUP: " << dir->ownerGroup(childName);
                        qDebug() << MODULE_NAME_STR << "\tPARENT: " << dir->parentUri();


                if (dir->mimetype(childName) == "inode/directory") {
                    Directory * childDir = m_FS.getDirectory(dir->childUri(childName));
                            qDebug() << MODULE_NAME_STR << " : " << childDir->uri() << " childs " << childDir->children();
                            m_FS.releaseDirectory(childDir);
                } else {

                }
            }
            m_FS.releaseDirectory(dir);
                    delete dirWatcher;
        });



        qDebug() << MODULE_NAME_STR << " : Testing VOLUME MANAGER";
        QList<Volume*> volumes = m_VolumeManager.volumes();
        for (Volume * volume : volumes) {
            qDebug() << MODULE_NAME_STR << " volume name" << volume->name();
            qDebug() << MODULE_NAME_STR << " volume icon" << volume->iconName();
            qDebug() << MODULE_NAME_STR << " volume automount?" << volume->automount();
            if (volume->automount()) {
                QFutureWatcher<Mount*> * mountWatcher = new QFutureWatcher<Mount*> ();
                mountWatcher->setFuture(volume->mount());

                QObject::connect(mountWatcher, &QFutureWatcher<Mount*>::finished, [mountWatcher] () {
                    qDebug() << MODULE_NAME_STR << " mount finished" << mountWatcher->progressText();
                    delete mountWatcher;
                });

                //                GVFSMount * mount = volume->getMount();
                //                if (mount != NULL) {
                //                    QFutureWatcher<void> * unmountWatcher = new QFutureWatcher<void> ();
                //                    unmountWatcher->setFuture(mount->unmount());
                //
                //                    QObject::connect(unmountWatcher, &QFutureWatcher<void>::finished, [unmountWatcher] () {
                //                        qDebug() << MODULE_NAME_STR << " unmount finished" << unmountWatcher->progressText();
                //                        delete unmountWatcher;
                //                    });
                //                }

                QFutureWatcher<void> * ejectWatcher = new QFutureWatcher<void> ();
                ejectWatcher->setFuture(volume->eject());

                QObject::connect(ejectWatcher, &QFutureWatcher<void>::finished, [ejectWatcher] () {
                    qDebug() << MODULE_NAME_STR << " volume ejected: " << ejectWatcher->progressText();
                    delete ejectWatcher;
                });

            }

        }

        int count = volumes.size();
        while (count--)
            delete volumes.takeAt(0);
        volumes.clear();

        QList<Mount*> mounts = m_VolumeManager.mounts();
        for (Mount * mount : mounts) {
            qDebug() << MODULE_NAME_STR << " mount name" << mount->name();
            qDebug() << MODULE_NAME_STR << " mount icon" << mount->iconName();
        }

        count = mounts.size();
        while (count--)
            delete mounts.takeAt(0);
        mounts.clear();



        qDebug() << MODULE_NAME_STR << " : Tests finished";
    }

    FileSystemGVFS m_FS;
    GVFSVolumeManager m_VolumeManager;
};
US_EXPORT_MODULE_ACTIVATOR(model_filesystem_gvfs, Activator)
