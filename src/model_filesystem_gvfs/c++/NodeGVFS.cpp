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


#include "module_config.h"
#include "NodeGVFS.h"

#include <glib.h>
#include <QObject>
#include <QDebug>
#include <QStringList>

NodeGVFS::NodeGVFS(QString path) : model_filesystem::Node(path) {
    m_File = g_file_new_for_path(path.toLocal8Bit());
    m_FileInfo = NULL;
    m_IsValid = g_file_query_exists(m_File, NULL);
}

NodeGVFS::NodeGVFS(QUrl uri) : model_filesystem::Node(uri) {
    m_File = g_file_new_for_uri(uri.toString().toLocal8Bit());
    m_FileInfo = NULL;
    m_IsValid = g_file_query_exists(m_File, NULL);
}

NodeGVFS::NodeGVFS(GFile *file) : model_filesystem::Node(QString()) {
    m_File = file;
    char * uri = g_file_get_uri(m_File);
    m_Uri = uri;
    g_free(uri);
    m_FileInfo = NULL;

    if (file == NULL) {
        qWarning() << MODULE_NAME_STR << " node created from NULL GFile.";
        m_IsValid = false;
    }
}

NodeGVFS::NodeGVFS(GFile *parent, GFileInfo* info) : model_filesystem::Node(QString()) {
    const char * name = g_file_info_get_name(info);
    m_File = g_file_get_child(parent, name);
    m_FileInfo = info;

    char * uri = g_file_get_uri(m_File);
    m_Uri = uri;
    m_IsValid = true;
    g_free(uri);
}

NodeGVFS::NodeGVFS(const NodeGVFS & orig) : model_filesystem::Node(orig) {
    m_Uri = orig.m_Uri;
    m_File = g_file_dup(orig.m_File);
    m_FileInfo = g_file_info_dup(orig.m_FileInfo);
    m_IsValid = true;
}

NodeGVFS::~NodeGVFS() {
    g_object_unref(m_FileInfo);
    g_object_unref(m_File);
}

QString NodeGVFS::name() {
    if (m_FileInfo == NULL)
        queryInfo("*");

    if (m_FileInfo == NULL) {
        qWarning() << MODULE_NAME_STR << ": unable to resolve file info for " << m_Uri;
        return QString();
    }

    QString name = g_file_info_get_display_name(m_FileInfo);
    return name;
}

void NodeGVFS::setName(const QString& name) {

    GError *error = NULL;
    GFile * new_file = g_file_set_display_name(m_File, name.toLocal8Bit(), NULL, &error);

    if (new_file == NULL) {
        qWarning() << MODULE_NAME_STR << " " << error->message;
        g_error_free(error);
        error = NULL;
    } else {
        g_object_unref(m_File);
        m_File = new_file;

        // Clear info cache
        g_object_unref(m_FileInfo);
        m_FileInfo = NULL;
    }
}

bool NodeGVFS::isValid() {
    return m_IsValid;
}

QString NodeGVFS::mimetype() {
    if (m_FileInfo == NULL)
        queryInfo("*");

    if (m_FileInfo == NULL) {
        qWarning() << MODULE_NAME_STR << ": unable to resolve file info for " << m_Uri;
        return QString();
    }

    QString mimeType = g_file_info_get_content_type(m_FileInfo);
    return mimeType;
}

QString NodeGVFS::iconName() {
    if (m_FileInfo == NULL)
        queryInfo("*");

    if (m_FileInfo == NULL) {
        qWarning() << MODULE_NAME_STR << ": unable to resolve file info for " << m_Uri;
        return QString();
    }

    GIcon *icon = NULL;
    int j;
    const gchar * const * names = NULL;

    icon = g_file_info_get_icon(m_FileInfo);

    if (icon == NULL)
        icon = g_file_info_get_symbolic_icon(m_FileInfo);

    if (G_IS_THEMED_ICON(icon)) {
        names = g_themed_icon_get_names(G_THEMED_ICON(icon));
    }
    if (names != NULL) {
        // Just return the first icon
        for (; (*names) != NULL; names++) {
            return QString::fromLocal8Bit((*names));
        }
    }
    g_object_unref(icon);
    return QString();
}

Node* NodeGVFS::parent() {
    GFile * parent = g_file_get_parent(m_File);
    return new NodeGVFS(parent);
    g_object_unref(parent);
}

QList<Node*> NodeGVFS::children() {
    QList<Node*> children;

    GFileEnumerator *enumerator;
    GFileInfo *info;
    GError *error;

    static gboolean nofollow_symlinks = FALSE;
    char attributes[] = "standard::*";

    if (m_File == NULL)
        return children;

    error = NULL;
    enumerator = g_file_enumerate_children(m_File,
            attributes,
            nofollow_symlinks ? G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS : G_FILE_QUERY_INFO_NONE,
            NULL,
            &error);
    if (enumerator == NULL) {
        qWarning() << MODULE_NAME_STR << " " << error->message;
        g_error_free(error);
        error = NULL;
        return children;
    }

    while ((info = g_file_enumerator_next_file(enumerator, NULL, &error)) != NULL) {
        children.append(new NodeGVFS(m_File, info));
        // The new Node takes control of the info, there is no need to release it.
    }

    if (error) {
        qWarning() << MODULE_NAME_STR << " " << error->message;
        g_error_free(error);
        error = NULL;
    }

    if (!g_file_enumerator_close(enumerator, NULL, &error)) {
        qWarning() << MODULE_NAME_STR << " " << error->message;
        g_error_free(error);
        error = NULL;
    }
    g_object_unref(enumerator);
    return children;
}

bool NodeGVFS::queryInfo(const char * attributes) {
    if (m_FileInfo)
        g_object_unref(m_FileInfo);

    GFileQueryInfoFlags flags;
    GError *error;

    if (m_File == NULL)
        return FALSE;

    // TODO: Extend query
    //    attributes = "standard::name,standard::display-name,standard::icon,standard::content-type,standard::symbolic-icon";

    qDebug() << "[" << MODULE_NAME_STR << "] Getting info :" << attributes;
    flags = G_FILE_QUERY_INFO_NONE;

    error = NULL;

    m_FileInfo = g_file_query_info(m_File, attributes, flags, NULL, &error);

    if (m_FileInfo == NULL) {
        qDebug() << "[" << MODULE_NAME_STR << "] [Error] getting info :" << error->message;
        g_error_free(error);
        return FALSE;
    }

    return TRUE;
}

QString NodeGVFS::localPath() {
    char * c_path = g_file_get_path(m_File);
    QString path = c_path;
    g_free(c_path);
    return path;
}
