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
#include "NodeGVFS.h"

#include <QString>
#include <QDebug>

FileSystemGVFS::FileSystemGVFS() : model_filesystem::FileSystem(), m_SupportedUriSchemes() {
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

Node* FileSystemGVFS::getNode(QString path) {
    QUrl url = QUrl::fromUserInput(path);
    if (m_SupportedUriSchemes.contains(url.scheme())) {
        Node * node = new NodeGVFS(url);
        return node;
    } else {
        qDebug() << "[" << MODULE_NAME_STR << "] not supported scheme: " << path;
        return NULL;
    }
    
}

QList<QAction> FileSystemGVFS::getActions(QList<Node*> nodes) {
    return QList<QAction>();
}
