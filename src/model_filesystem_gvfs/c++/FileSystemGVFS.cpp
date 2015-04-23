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

#include <QString>
#include <QDebug>

FileSystemGVFS::FileSystemGVFS() : /* model_filesystem::FileSystem(),*/ m_SupportedUriSchemes() {
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

GVFSDirectory* FileSystemGVFS::getDirectory(const QString& uri) {
    QString realUri = uri;
    if (realUri.at(uri.size() - 1) != '/')
        realUri.append('/');

    qDebug() << "dirs in cache: " << m_Cache.keys();
    if (m_Cache.contains(realUri)) {
        GVFSDirectory * dir = m_Cache.value(realUri);
        m_Refs[dir]++;
        qDebug() << MODULE_NAME_STR << realUri << "  fetched from cache with" << m_Refs[dir] << " references.";
        return dir;
    } else {
        GVFSDirectory * dir = new GVFSDirectory(realUri);
        qDebug() << MODULE_NAME_STR << realUri << " fetched from fs.";
        m_Cache.insert(realUri, dir);
        m_Refs.insert(dir, 1);
        return dir;
    }
}

void FileSystemGVFS::releaseDirectory(GVFSDirectory* dir) {
    QString uri = dir->uri();

    if (m_Refs.contains(dir)) {
        m_Refs[dir]--;
        qDebug() << MODULE_NAME_STR << " " << uri << " references decreased to: " << m_Refs[dir];
        if (m_Refs[dir] <= 0) {
            m_Refs.remove(dir);
            delete m_Cache.take(uri);
            qDebug() << MODULE_NAME_STR << " " << uri << " released.";
        }
    } else {
        qDebug() << MODULE_NAME_STR << uri << " is not in cache.";
    }
}

