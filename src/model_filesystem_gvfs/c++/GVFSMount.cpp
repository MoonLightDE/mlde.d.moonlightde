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

#include "GVFSMount.h"
#include "GVFSDirectory.h"
#include "UnmountMountOp.h"

#include <QFutureInterface>
#include <QDebug>

GVFSMount::GVFSMount(GMount * gmount) : QObject(), m_Operation(NULL) {
    m_GMount = G_MOUNT(gmount);
}

GVFSMount::~GVFSMount() {
    g_object_unref(m_GMount);
}

QString GVFSMount::name() {
    gchar * cname = g_mount_get_name(m_GMount);
    QString name(cname);
    g_free(cname);
    return name;
}

QString GVFSMount::uuid() {
    gchar * cuuid = g_mount_get_uuid(m_GMount);
    QString uuid(cuuid);
    g_free(cuuid);
    return uuid;
}

QString GVFSMount::iconName() {
    GIcon *gicon = g_mount_get_icon(m_GMount);

    const gchar * const * names = g_themed_icon_get_names(G_THEMED_ICON(gicon));
    QString name;
    if (names != NULL) {
        // Just return the first icon
        for (; (*names) != NULL; names++) {
            name = QString::fromLocal8Bit(*names);
            break;
        }
    }
    g_object_unref(gicon);
    return name;
}

GVFSDirectory* GVFSMount::root() {
    GVFSDirectory *dir = new GVFSDirectory(g_mount_get_root(m_GMount));
    return dir;
}

GVFSVolume* GVFSMount::volume() {
    GVolume * gvolume = g_mount_get_volume(m_GMount);
    if (gvolume)
        return new GVFSVolume(gvolume);
}

QFuture<void> GVFSMount::unmount() {
    UnmountMountOp *umountOp = new UnmountMountOp(m_GMount);
    return umountOp->run();
}

QFuture<void> GVFSMount::eject() {
    if (m_Operation != NULL)
        return m_Operation->future();

    m_Operation = new QFutureInterface<void>();

    m_Operation->reportStarted();

    GMountOperation * mount_op;

    mount_op = g_mount_operation_new();

    g_signal_connect(mount_op, "ask_password", G_CALLBACK(handleAskPassword), NULL);

    g_mount_eject_with_operation(m_GMount, G_MOUNT_UNMOUNT_NONE, mount_op, NULL, handleEjectFinish, this);
    g_object_unref(mount_op);
}

bool GVFSMount::removable() {
    return g_mount_can_unmount(m_GMount);
}

bool GVFSMount::ejectable() {
    return g_mount_can_eject(m_GMount);
}

void GVFSMount::handleAskPassword(GMountOperation* op, gchar* message, gchar* default_user, gchar* default_domain, GAskPasswordFlags flags, GVFSMount * volume) {
    volume->m_Operation->setProgressValueAndText(-1, "Requesting autentication.");
    // TODO: Conect to the authentication service
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet";
}

void GVFSMount::handleUmountFinish(GObject* object, GAsyncResult* res, gpointer userdata) {
    GVFSMount * mount = static_cast<GVFSMount *> (userdata);
    if (!mount) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }
    GError *error = NULL;
    gboolean succeeded;
    GMount * gmount = G_MOUNT(object);

    succeeded = g_mount_unmount_with_operation_finish(gmount, res, &error);

    if (!succeeded) {
        mount->m_Operation->setProgressValueAndText(-1, QString("Unable to unmount %1, due %2").arg(g_mount_get_name(gmount)).arg(error->message));
        mount->m_Operation->reportFinished();
        g_object_unref(error);
    } else
        mount->m_Operation->reportFinished();
}

void GVFSMount::handleRemountFinish(GObject* object, GAsyncResult* res, gpointer userdata) {
    GVFSMount * mount = static_cast<GVFSMount *> (userdata);
    if (!mount) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }
    GError *error = NULL;
    gboolean succeeded;
    GMount * gmount = G_MOUNT(object);

    succeeded = g_mount_remount_finish(gmount, res, &error);

    if (!succeeded) {
        mount->m_Operation->setProgressValueAndText(-1, QString("Unable to unmount %1, due %2").arg(g_mount_get_name(gmount)).arg(error->message));
        mount->m_Operation->reportFinished();
        g_object_unref(error);
    } else
        mount->m_Operation->reportFinished();
}

void GVFSMount::handleEjectFinish(GObject* object, GAsyncResult* res, gpointer userdata) {
    GVFSMount * mount = static_cast<GVFSMount *> (userdata);
    if (!mount) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }
    GError *error = NULL;
    gboolean succeeded;
    GMount * gmount = G_MOUNT(object);

    succeeded = g_mount_eject_with_operation_finish(gmount, res, &error);

    if (!succeeded) {
        mount->m_Operation->setProgressValueAndText(-1, QString("Unable to unmount %1, due %2").arg(g_mount_get_name(gmount)).arg(error->message));
        mount->m_Operation->reportFinished();
        g_object_unref(error);
    } else
        mount->m_Operation->reportFinished();
}

