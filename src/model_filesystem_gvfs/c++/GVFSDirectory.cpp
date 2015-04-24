/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 *      Ruben Salvador San Juan
 *      Jorge Damian Diaz Morejon 
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

#include <QString>
#include <QFile>
#include <QFileDevice>
#include <QUrl>
#include <QDebug>

char GVFSDirectoryPriv::attributes[] = "standard::*,access::*,owner::*,time::*";
bool GVFSDirectoryPriv::nofollow_symlinks = false;

QHash<QString, GVFSDirectoryPriv*>* GVFSDirectory::m_Cache = NULL;
QHash<GVFSDirectoryPriv*, int>* GVFSDirectory::m_Refs = NULL;

GVFSDirectory::GVFSDirectory(const QString &uri) {
    if (m_Cache == NULL)
        m_Cache = new QHash<QString, GVFSDirectoryPriv*>();

    if (m_Refs == NULL)
        m_Refs = new QHash<GVFSDirectoryPriv*, int>();

    QUrl qurl = QUrl::fromUserInput(uri);
    QString cleanUri = qurl.toString(QUrl::RemoveUserInfo | QUrl::PreferLocalFile | QUrl::StripTrailingSlash | QUrl::NormalizePathSegments);

    if (m_Cache->contains(cleanUri)) {
        m_Priv = m_Cache->value(cleanUri);
        (*m_Refs)[m_Priv]++;
    } else {
        GFile *gfile = g_file_new_for_commandline_arg(cleanUri.toLocal8Bit());
        Q_ASSERT(G_FILE(gfile));
        m_Priv = new GVFSDirectoryPriv(gfile);

        m_Priv->m_User = qurl.userName();
        m_Priv->m_Password = qurl.password();

        update();
    }
    connect(m_Priv, &GVFSDirectoryPriv::changed, this, &GVFSDirectory::changed);
    connect(m_Priv, &GVFSDirectoryPriv::failure, this, &GVFSDirectory::failure);
}

GVFSDirectory::GVFSDirectory(GFile * gfile) {
    m_Priv->m_File = G_FILE(gfile);
    m_Priv->m_uri = g_file_get_uri(m_Priv->m_File);
    m_Priv->m_FileInfo = NULL;

    update();
}

GVFSDirectory::~GVFSDirectory() {
    disconnect(m_Priv, NULL, this, NULL);
    if ((--(*m_Refs)[m_Priv]) <= 0)
        delete m_Priv;
}

QString GVFSDirectory::name() {
    if (m_Priv->m_FileInfo == NULL)
        return QString();

    QString name = g_file_info_get_display_name(m_Priv->m_FileInfo);
    return name;
}

QString GVFSDirectory::uri() {
    return m_Priv->m_uri;
}

QString GVFSDirectory::parentUri() {
    GFile * parent = g_file_get_parent(m_Priv->m_File);
    char* uri = g_file_get_uri(parent);

    QString uriStr(uri);
    g_free(uri);
    return uriStr;
}

QString GVFSDirectory::mimetype(const QString& child) {
    if (m_Priv->m_ChildrenInfo.contains(child)) {
        GFileInfo * childInfo = m_Priv->m_ChildrenInfo.value(child);
        QString mimeType = g_file_info_get_content_type(childInfo);
        return mimeType;
    } else
        return QString();
}

QString GVFSDirectory::iconName(const QString& child) {
    GFileInfo *fileInfo = m_Priv->m_ChildrenInfo.value(child, NULL);
    if (G_FILE_INFO(fileInfo)) {
        GIcon *gicon = g_file_info_get_icon(fileInfo);

        const gchar * const * names = g_themed_icon_get_names(G_THEMED_ICON(gicon));
        QString name;
        if (names != NULL) {
            // Just return the first icon
            for (; (*names) != NULL; names++) {
                name = QString::fromLocal8Bit(*names);
                break;
            }
        }
        return name;
    } else
        return QString();
}

QFlags<QFile::Permission> GVFSDirectory::permission(const QString& target) {
    if (m_Priv->m_ChildrenInfo.contains(target)) {
        GFileInfo* file = m_Priv->m_ChildrenInfo.value(target);

        bool canRead = g_file_info_get_attribute_boolean(file, G_FILE_ATTRIBUTE_ACCESS_CAN_READ);
        bool canWrite = g_file_info_get_attribute_boolean(file, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE);
        bool canExec = g_file_info_get_attribute_boolean(file, G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE);

        QFile::Permissions permissions;
        if (canRead)
            permissions |= QFile::ReadOwner;
        if (canWrite)
            permissions |= QFile::WriteOwner;
        if (canExec)
            permissions |= QFile::ExeOwner;

        return permissions;

    }

    return QFile::Permission();
}

//TODO: Fix this method because it's not recursive 
//and it does not apply permissions to file

QFuture<void> GVFSDirectory::setPermission(QFlags<QFile::Permission> permissions, const QString& target) {
    return QFuture<void>();

    // TODO: Move current implementation to a separated Operation.
    if (m_Priv->m_ChildrenInfo.contains(target)) {
        GFileInfo* file = m_Priv->m_ChildrenInfo.value(target);

        bool canRead = permissions.testFlag(QFile::ReadOwner);
        bool canWrite = permissions.testFlag(QFile::WriteOwner);
        bool canExec = permissions.testFlag(QFile::ExeOwner);
        gpointer ptrCanRead = &canRead;
        gpointer ptrCanWrite = &canWrite;
        gpointer ptrCanExec = &canExec;

        if (canRead && canWrite && canExec) {
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_READ, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanRead);
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanWrite);
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanExec);
        } else if (canRead && canWrite) {
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_READ, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanRead);
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanWrite);
        } else if (canRead && canExec) {
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_READ, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanRead);
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanExec);
        } else if (canWrite && canExec) {
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanExec);
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanWrite);
        } else if (canRead) {
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_READ, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanRead);
        } else if (canWrite) {
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanWrite);
        } else if (canExec) {
            g_file_info_set_attribute(file, G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE, G_FILE_ATTRIBUTE_TYPE_BOOLEAN, ptrCanExec);
        }
    }
}

QFuture<unsigned long int> GVFSDirectory::size(const QString& target, bool recursive) {
    QFutureInterface<unsigned long int> fi;
    fi.reportResult(new unsigned long int(0));
    return fi.future();
    // TODO: Move current implementation to a separated Operation.

    // FIXME: Code bellow calls to GVFSDirectory destructor with out taking care of 
    // the callbacks and signals conected to then and cause segmentation faults.
    long int sizes = 0;
    GVFSDirectory* current;
    if (target == ".") {
        current = this;
        if (recursive) {
            for (QString childTarget : children()) {
                if (g_file_info_get_file_type(m_Priv->m_ChildrenInfo.value(childTarget)) == G_FILE_TYPE_DIRECTORY) {
                    current = new GVFSDirectory(childUri(childTarget));
                    sizes += current->size(".", true);
                    delete current;
                } else {
                    sizes += g_file_info_get_size(m_Priv->m_ChildrenInfo.value(childTarget));
                }
            }

        } else {
            for (GFileInfo* info : m_Priv->m_ChildrenInfo) {
                if (g_file_info_get_file_type(info) != G_FILE_TYPE_DIRECTORY) {
                    sizes += g_file_info_get_size(info);
                }
            }
            delete current;
        }

    } else {
        if (m_Priv->m_ChildrenInfo.contains(target)) {
            if (g_file_info_get_file_type(m_Priv->m_ChildrenInfo.value(target)) == G_FILE_TYPE_DIRECTORY) {
                current = new GVFSDirectory(childUri(target));
                if (recursive) {
                    sizes += current->size(".", true);
                    delete current;
                } else {
                    for (QString childTarget : current->m_Priv->m_ChildrenInfo.keys()) {
                        if (g_file_info_get_file_type(current->m_Priv->m_ChildrenInfo.value(childTarget)) != G_FILE_TYPE_DIRECTORY) {
                            sizes += g_file_info_get_size(current->m_Priv->m_ChildrenInfo.value(childTarget));
                        }
                    }
                    delete current;
                }

            } else {

                sizes += g_file_info_get_size(m_Priv->m_ChildrenInfo.value(target));
            }

        }

    }

    //    return sizes;

}

QFuture<unsigned long int> GVFSDirectory::storedSize(const QString& target, bool recursive) {
    QFutureInterface<unsigned long int> fi;
    fi.reportResult(new unsigned long int(0));
    return fi.future();
    // TODO: Move current implementation to a separated Operation.

    // FIXME: Code bellow calls to GVFSDirectory destructor with out taking care of 
    // the callbacks and signals conected to then and cause segmentation faults.
    unsigned long int sizes = 0;
    GVFSDirectory* current;
    if (target == ".") {
        current = this;
        if (recursive) {
            for (QString childTarget : children()) {
                if (g_file_info_get_file_type(m_Priv->m_ChildrenInfo.value(childTarget)) == G_FILE_TYPE_DIRECTORY) {
                    current = new GVFSDirectory(childUri(childTarget));
                    sizes += current->size(".", true);
                    delete current;
                } else {
                    sizes += g_file_info_get_attribute_uint64(m_Priv->m_ChildrenInfo.value(childTarget), G_FILE_ATTRIBUTE_STANDARD_ALLOCATED_SIZE);
                }
            }

        } else {
            for (GFileInfo* info : m_Priv->m_ChildrenInfo) {
                if (g_file_info_get_file_type(info) != G_FILE_TYPE_DIRECTORY) {
                    sizes += g_file_info_get_attribute_uint64(info, G_FILE_ATTRIBUTE_STANDARD_ALLOCATED_SIZE);
                }
            }
            delete current;
        }

    } else {
        if (m_Priv->m_ChildrenInfo.contains(target)) {
            if (g_file_info_get_file_type(m_Priv->m_ChildrenInfo.value(target)) == G_FILE_TYPE_DIRECTORY) {
                current = new GVFSDirectory(childUri(target));
                if (recursive) {
                    sizes += current->size(".", true);
                    delete current;
                } else {
                    for (QString childTarget : current->m_Priv->m_ChildrenInfo.keys()) {
                        if (g_file_info_get_file_type(current->m_Priv->m_ChildrenInfo.value(childTarget)) != G_FILE_TYPE_DIRECTORY) {
                            sizes += g_file_info_get_attribute_uint64(m_Priv->m_ChildrenInfo.value(childTarget), G_FILE_ATTRIBUTE_STANDARD_ALLOCATED_SIZE);
                        }
                    }
                    delete current;
                }

            } else {

                sizes += g_file_info_get_attribute_uint64(m_Priv->m_ChildrenInfo.value(target), G_FILE_ATTRIBUTE_STANDARD_ALLOCATED_SIZE);
            }

        }

    }

    //    return sizes;
}

unsigned long int GVFSDirectory::timeAccess(const QString& target) {
    long long sec = 0;

    if (target == ".") {
        sec = g_file_info_get_attribute_uint64(m_Priv->m_FileInfo, G_FILE_ATTRIBUTE_TIME_ACCESS);
    } else {
        if (m_Priv->m_ChildrenInfo.contains(target)) {
            sec = g_file_info_get_attribute_uint64(m_Priv->m_ChildrenInfo.value(target), G_FILE_ATTRIBUTE_TIME_ACCESS);
        } else
            return 0;
    }

    return sec;
}

unsigned long int GVFSDirectory::timeModified(const QString& target) {

    long int sec;

    if (target != ".") {

        if (m_Priv->m_ChildrenInfo.contains(target)) {
            sec = g_file_info_get_attribute_uint64(m_Priv->m_ChildrenInfo.value(target), G_FILE_ATTRIBUTE_TIME_MODIFIED);
        } else
            return 0;

    } else {

        sec = g_file_info_get_attribute_uint64(m_Priv->m_FileInfo, G_FILE_ATTRIBUTE_TIME_MODIFIED);
    }

    return sec;
}

unsigned long int GVFSDirectory::timeChanged(const QString& target) {

    long int sec;
    if (target != ".") {

        if (children().contains(target)) {
            sec = g_file_info_get_attribute_uint64(m_Priv->m_ChildrenInfo.value(target), G_FILE_ATTRIBUTE_TIME_CHANGED);
        } else
            return 0;

    } else {

        sec = g_file_info_get_attribute_uint64(m_Priv->m_FileInfo, G_FILE_ATTRIBUTE_TIME_CHANGED);
    }

    return sec;
}

QString GVFSDirectory::childUri(const QString& name) {
    GFile * child = g_file_get_child(m_Priv->m_File, name.toLocal8Bit());
    char *childUri = g_file_get_uri(child);

    QString childUriStr = childUri;
    g_free(childUri);
    g_object_unref(child);

    return childUriStr;
}

QString GVFSDirectory::ownerUser(const QString& target) {

    QString user;
    if (target == ".") {
        user = g_file_info_get_attribute_as_string(m_Priv->m_FileInfo, G_FILE_ATTRIBUTE_OWNER_USER);
    } else {

        if (children().contains(target)) {
            user = g_file_info_get_attribute_as_string(m_Priv->m_ChildrenInfo.value(target), G_FILE_ATTRIBUTE_OWNER_USER);
        } else {
            return QString();
        }

    }

    return user;
}

QString GVFSDirectory::ownerGroup(const QString& target) {

    QString group;
    if (target == ".") {
        group = g_file_info_get_attribute_as_string(m_Priv->m_FileInfo, G_FILE_ATTRIBUTE_OWNER_GROUP);
    } else {

        if (children().contains(target)) {
            group = g_file_info_get_attribute_as_string(m_Priv->m_ChildrenInfo.value(target), G_FILE_ATTRIBUTE_OWNER_GROUP);
        } else {
            return QString();
        }

    }

    return group;
}

int GVFSDirectory::childernCount() {
    return m_Priv->m_ChildrenInfo.size();
}

QList<QString> GVFSDirectory::children() {
    QList<QString> children;
    for (GFileInfo * fileInfo : m_Priv->m_ChildrenInfo) {
        const char * cchildName = g_file_info_get_name(fileInfo);
        children.append(QString(cchildName));
    }
    return children;
}

void GVFSDirectory::update() {
    m_Priv->clear();
    m_Priv->startMount();
}

QFuture<void> GVFSDirectory::status() {
    return m_Priv->m_FutureInterface.future();
}

void GVFSDirectoryPriv::startMount() {
    if (!G_FILE(m_File)) {
        reportError("Invalid directory.");
        qWarning() << MODULE_NAME_STR << " working over an invalid gfile reference this may be an error, plase report it.";
        return;
    }

    GMountOperation *op;
    op = g_mount_operation_new();

    g_signal_connect(op, "ask_password", G_CALLBACK(handleMountAskPassword), this);

    m_FutureInterface.reportStarted();
    m_FutureInterface.setProgressValueAndText(0, "Mounting file system.");

    g_file_mount_enclosing_volume(m_File, G_MOUNT_MOUNT_NONE, op, NULL, handleMountDone, this);
}

void GVFSDirectoryPriv::handleMountAskPassword(GMountOperation* op, gchar* message, gchar* default_user, gchar* default_domain, GAskPasswordFlags flags, gpointer userdata) {
    GVFSDirectoryPriv * directoryPriv = static_cast<GVFSDirectoryPriv *> (userdata);
    if (!directoryPriv) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }
    directoryPriv->m_FutureInterface.setProgressValueAndText(1, "Waithing for user credentials.");

    // TODO: Implement using the authentication service
    g_mount_operation_set_username(op, directoryPriv->m_User.toLocal8Bit());
    g_mount_operation_set_password(op, directoryPriv->m_Password.toLocal8Bit());

    g_mount_operation_reply(op, G_MOUNT_OPERATION_HANDLED);
}

void GVFSDirectoryPriv::handleMountDone(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GVFSDirectoryPriv * directoryPriv = static_cast<GVFSDirectoryPriv *> (user_data);
    if (!directoryPriv) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }

    GError *error = NULL;
    gboolean succeeded;

    succeeded = g_file_mount_enclosing_volume_finish(G_FILE(source_object), res, &error);

    if (!succeeded) {
        if ((error->code == G_IO_ERROR_ALREADY_MOUNTED) || (error->code == G_IO_ERROR_NOT_SUPPORTED)) {
            directoryPriv->m_FutureInterface.setProgressValueAndText(2, QString::fromLocal8Bit(error->message));

            directoryPriv->startQueryFileInfo();
        } else {
            directoryPriv->reportError(QString::fromLocal8Bit(error->message));
        }
        g_error_free(error);
    } else {
        directoryPriv->m_FutureInterface.setProgressValueAndText(2, "Mounted.");

        directoryPriv->startQueryFileInfo();
    }
}

void GVFSDirectoryPriv::startQueryFileInfo() {
    clear();
    m_FutureInterface.setProgressValueAndText(3, "Querying directory info.");

    g_file_query_info_async(m_File, attributes, G_FILE_QUERY_INFO_NONE, G_PRIORITY_DEFAULT, NULL, handleQueryFileInfoDone, this);
}

void GVFSDirectoryPriv::handleQueryFileInfoDone(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GVFSDirectoryPriv * directoryPriv = static_cast<GVFSDirectoryPriv *> (user_data);
    if (!directoryPriv) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }

    if (directoryPriv->m_FileInfo != NULL)
        directoryPriv->clear();

    GError *error = NULL;
    directoryPriv->m_FileInfo = g_file_query_info_finish(G_FILE(source_object), res, &error);
    if (directoryPriv->m_FileInfo == NULL) {
        directoryPriv->reportError(QString::fromLocal8Bit(error->message));
        g_error_free(error);
    } else {
        directoryPriv->m_FutureInterface.setProgressValueAndText(4, "Directory info ready.");

        directoryPriv->startEnumerateChildren();
    }
}

void GVFSDirectoryPriv::startEnumerateChildren() {
    m_FutureInterface.setProgressValueAndText(5, "Enumerating children.");

    g_file_enumerate_children_async(m_File,
            attributes,
            nofollow_symlinks ? G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS : G_FILE_QUERY_INFO_NONE, 0,
            NULL,
            handleEnumerateChildrenDone,
            this);
}

void GVFSDirectoryPriv::handleEnumerateChildrenDone(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GVFSDirectoryPriv * directoryPriv = static_cast<GVFSDirectoryPriv *> (user_data);
    if (!directoryPriv) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }

    GFileEnumerator *enumerator;
    GError *error = NULL;

    enumerator = g_file_enumerate_children_finish(G_FILE(source_object), res, &error);

    if (enumerator == NULL) {
        directoryPriv->reportError(QString::fromLocal8Bit(error->message));
        g_error_free(error);
        error = NULL;
    } else {
        g_file_enumerator_next_files_async(enumerator, 20, 0, NULL, handleEnumeratorNextFilesDone, directoryPriv);
    }
}

void GVFSDirectoryPriv::handleEnumeratorNextFilesDone(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GVFSDirectoryPriv * directoryPriv = static_cast<GVFSDirectoryPriv *> (user_data);
    if (!directoryPriv) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }

    GFileEnumerator *enumerator = G_FILE_ENUMERATOR(source_object);
    GError *error = NULL;

    GList * infoList = g_file_enumerator_next_files_finish(enumerator, res, &error);
    if (error) {
        directoryPriv->reportError(QString::fromLocal8Bit(error->message));
        g_error_free(error);
        g_list_free(infoList);

        g_file_enumerator_close_async(enumerator, 0, NULL, handleEnumeratorCloseDone, user_data);
    } else {
        if (infoList == NULL) {
            g_file_enumerator_close_async(enumerator, 0, NULL, handleEnumeratorCloseDone, directoryPriv);
        } else {
            GList * ptr = infoList;
            while (ptr != NULL) {
                GFileInfo * fileInfo = static_cast<GFileInfo *> (ptr->data);
                directoryPriv->m_ChildrenInfo.insert(QString::fromLocal8Bit(g_file_info_get_name(fileInfo)), g_file_info_dup(fileInfo));
                ptr = ptr->next;
            }

            g_list_free(infoList);
            Q_EMIT(directoryPriv->changed());
            g_file_enumerator_next_files_async(enumerator, 20, 0, NULL, handleEnumeratorNextFilesDone, directoryPriv);
        }
    }
}

void GVFSDirectoryPriv::handleEnumeratorCloseDone(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GVFSDirectoryPriv * directoryPriv = static_cast<GVFSDirectoryPriv *> (user_data);
    if (!directoryPriv) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }

    GFileEnumerator *enumerator = G_FILE_ENUMERATOR(source_object);
    GError *error;

    if (!g_file_enumerator_close_finish(enumerator, res, &error)) {
        if (error->code != G_IO_ERROR_CLOSED)
            directoryPriv->reportError(QString::fromLocal8Bit(error->message));

        g_object_unref(enumerator);
        g_error_free(error);
    }

    directoryPriv->m_FutureInterface.reportFinished();

    if (directoryPriv->m_FutureInterface.progressValue() != -1) {
        directoryPriv->m_FutureInterface.setProgressValueAndText(6, "Child enumeration done.");
    } else
        Q_EMIT(directoryPriv->changed());
}

void GVFSDirectoryPriv::reportError(const QString& msg) {
    //    qWarning() << MODULE_NAME_STR << " unable to retrieve directory due: " << msg;
    m_FutureInterface.setProgressValueAndText(-1, msg);
    m_FutureInterface.reportFinished();

    Q_EMIT(failure(msg));
}

GVFSDirectoryPriv::GVFSDirectoryPriv(GFile * gfile) : QObject(), m_File(gfile), m_FileInfo(NULL) {
}

GVFSDirectoryPriv::~GVFSDirectoryPriv() {
    clear();
    g_object_unref(m_File);
}

void GVFSDirectoryPriv::clear() {
    for (GFileInfo * info : m_ChildrenInfo)
        g_object_unref(info);

    m_ChildrenInfo.clear();

    if (m_FileInfo != NULL) {
        g_object_unref(m_FileInfo);

        m_FileInfo = NULL;
    }
}
