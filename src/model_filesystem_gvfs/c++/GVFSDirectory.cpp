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
#include "GVFSDirectory.h"

#include <QMutexLocker>
#include <QFile>
#include <QDebug>

GVFSDirectory::GVFSDirectory(const QString &uri) : QObject() {
    m_uri = uri;
    m_File = g_file_new_for_uri(m_uri.toLocal8Bit());
    m_FileInfo = NULL;

    updateCache();
}

GVFSDirectory::GVFSDirectory(GFile * gfile) : QObject() {
    m_File = G_FILE(gfile);
    m_uri = g_file_get_uri(m_File);
    m_FileInfo = NULL;

    updateCache();
}

GVFSDirectory::~GVFSDirectory() {
    releaseCache();

    g_object_unref(m_File);
    m_File = NULL;

}

QString GVFSDirectory::name() {
    if (m_FileInfo == NULL)
        return QString();

    QString name = g_file_info_get_display_name(m_FileInfo);
    return name;
}

QString GVFSDirectory::uri() {
    return m_uri;
}

QString GVFSDirectory::parentUri() {
    GFile * parent = g_file_get_parent(m_File);
    char* uri = g_file_get_uri(parent);

    QString uriStr(uri);
    g_free(uri);
    return uriStr;
}

QString GVFSDirectory::mimetype(const QString& child) {
    if (m_ChildrenInfo.contains(child)) {
        GFileInfo * childInfo = m_ChildrenInfo.value(child);
        QString mimeType = g_file_info_get_content_type(childInfo);
        return mimeType;
    } else
        return QString();
}

QString GVFSDirectory::iconName(const QString& child) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
    return QString();

}

QFlags<QFile::Permission> GVFSDirectory::permission(const QString& target) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
    return QFlags<QFile::Permission>();
}

void GVFSDirectory::setPermission(QFlags<QFile::Permission> permissions, const QString& target) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
}

unsigned long int GVFSDirectory::size(const QString& target, bool recursive) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
    return 0;
}

unsigned long int GVFSDirectory::storedSize(const QString& target, bool recursive) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
    return 0;
}

unsigned long int GVFSDirectory::timeAccess(const QString& target) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
    return 0;
}

unsigned long int GVFSDirectory::timeModified(const QString& target) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
    return 0;
}

unsigned long int GVFSDirectory::timeChanged(const QString& target) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
    return 0;
}

QString GVFSDirectory::childUri(const QString& name) {
    GFile * child = g_file_get_child(m_File, name.toLocal8Bit());
    char *childUri = g_file_get_uri(child);

    QString childUriStr = childUri;
    g_free(childUri);
    g_object_unref(child);

    return childUriStr;
}

QString GVFSDirectory::ownerUser(const QString& target) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
    return QString();
}

QString GVFSDirectory::ownerGroup(const QString& target) {
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet.";
    return QString();
}

int GVFSDirectory::childernCount() {
    return m_ChildrenInfo.size();
}

QList<QString> GVFSDirectory::childern() {
    QList<QString> children;
    for (GFileInfo * fileInfo : m_ChildrenInfo) {
        const char * cchildName = g_file_info_get_name(fileInfo);
        children.append(QString(cchildName));
    }
    return children;
}

int GVFSDirectory::uid() {
    return qHash(m_uri);
}

void GVFSDirectory::updateCache() {
    emit(fetchingData());

    GFileInfo * fileInfo = queryDirectoryInfo();
    if (!fileInfo)
        return;

    QHash<QString, GFileInfo *> childrenInfo = queryChildrenInfo();

    // Update cache
    QMutexLocker mutexLocker(&m_Mutex);
    releaseCache();

    m_FileInfo = fileInfo;
    m_ChildrenInfo = childrenInfo;

    emit(dataReady());
}

void GVFSDirectory::releaseCache() {
    for (GFileInfo * info : m_ChildrenInfo) {
        g_object_unref(info);
    }
    m_ChildrenInfo.clear();

    if (m_FileInfo != NULL) {
        g_object_unref(m_FileInfo);
        m_FileInfo = NULL;
    }
}

GFileInfo* GVFSDirectory::queryDirectoryInfo() {
    GFileInfo * fileInfo = NULL;
    QList<GFileInfo *> childrenInfo;

    GFileQueryInfoFlags flags;
    GError *errorResult;

    if (m_File == NULL)
        return fileInfo;

    //TODO: Reduce query to only the attributes exposed by Directory.
    const char * attributes = "*";
    qDebug() << MODULE_NAME_STR << " Getting info :" << attributes;
    flags = G_FILE_QUERY_INFO_NONE;

    // Query directory info
    errorResult = NULL;
    fileInfo = g_file_query_info(m_File, attributes, flags, NULL, &errorResult);
    if (fileInfo == NULL) {
        qDebug() << MODULE_NAME_STR << " [Error] " << errorResult->message << " with code" << errorResult->code;
        emit(error(errorResult->message));
        g_error_free(errorResult);
    }

    return fileInfo;
}

QHash<QString, GFileInfo *> GVFSDirectory::queryChildrenInfo() {
    QHash<QString, GFileInfo *> children;

    GFileEnumerator *enumerator;
    GFileInfo *info;
    GError *errorReturn;

    static gboolean nofollow_symlinks = FALSE;
    char attributes[] = "standard::*";

    if (m_File == NULL)
        return children;

    errorReturn = NULL;
    enumerator = g_file_enumerate_children(m_File,
            attributes,
            nofollow_symlinks ? G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS : G_FILE_QUERY_INFO_NONE,
            NULL,
            &errorReturn);
    if (enumerator == NULL) {
        qWarning() << MODULE_NAME_STR << " " << errorReturn->message;
        emit(error(errorReturn->message));
        g_error_free(errorReturn);
        errorReturn = NULL;
        return children;
    }

    while ((info = g_file_enumerator_next_file(enumerator, NULL, &errorReturn)) != NULL) {
        const char * childName = g_file_info_get_name(info);
        children.insert(QString(childName), info);
    }

    if (errorReturn) {
        qWarning() << MODULE_NAME_STR << " " << errorReturn->message;
        emit(error(errorReturn->message));
        g_error_free(errorReturn);
        errorReturn = NULL;
    }

    if (!g_file_enumerator_close(enumerator, NULL, &errorReturn)) {
        qWarning() << MODULE_NAME_STR << " " << errorReturn->message;
        emit(error(errorReturn->message));
        g_error_free(errorReturn);
        errorReturn = NULL;
    }
    g_object_unref(enumerator);
    return children;
}

