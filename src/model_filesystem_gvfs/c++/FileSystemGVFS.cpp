/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
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

#include <gio/gio.h>

#include "module_config.h"
#include "FileSystemGVFS.h"
#include "GVFSDirectory.h"
#include "MountVolumeOp.h"
#include "GVFSDirectory.h"

#include "FileMoveOp.h"

#include "model_filesystem/VolumeManager.h"

#include <QString>
#include <QDebug>
#include <qt5/QtCore/qlogging.h>

FileSystemGVFS::FileSystemGVFS() : m_SupportedUriSchemes() {
    m_GVfs = g_vfs_get_default();
    const gchar * const * schemes = g_vfs_get_supported_uri_schemes(m_GVfs);
    while (*schemes != NULL) {
        m_SupportedUriSchemes.append(QString(*schemes));
        schemes++;
    }
}

FileSystemGVFS::~FileSystemGVFS() {
}

QStringList FileSystemGVFS::getSupportedUriScheme() {
    return m_SupportedUriSchemes;
}

/*
QList<QAction> FileSystemGVFS::getActions(QList<Node*> nodes) {
    return QList<QAction>();
}*/

model_filesystem::Directory* FileSystemGVFS::getDirectory(const QString& uri) {
    QString realUri = uri;
    //    if (realUri.at(uri.size() - 1) != '/')
    //        realUri.append('/');

    GVFSDirectory * dir = new GVFSDirectory(realUri);
    return dynamic_cast<model_filesystem::Directory*> (dir);
}

void FileSystemGVFS::releaseDirectory(model_filesystem::Directory* dir) {
}

QFuture<void> FileSystemGVFS::move(QList<QString> sources, QString destination) {
    FileMoveOp * op = new FileMoveOp(sources, destination);
    op->run();
    return op->status();
}
