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

#include "GVFSVolume.h"
#include "GVFSMount.h"
#include <QFutureInterface>

GVFSVolume::GVFSVolume(GVolume * gvolume) : QObject(), m_GVolume(gvolume), m_FutureMount(NULL) {
    Q_ASSERT(m_GVolume);
}

GVFSVolume::~GVFSVolume() {
    g_object_unref(m_GVolume);
}

QString GVFSVolume::name() {
    gchar * cname = g_volume_get_name(m_GVolume);

    QString name(cname);

    g_free(cname);
    return name;
}

QString GVFSVolume::uuid() {
    gchar * cuuid = g_volume_get_uuid(m_GVolume);

    QString uuid(cuuid);

    g_free(cuuid);
    return uuid;
}

QString GVFSVolume::iconName() {
    GIcon *gicon = g_volume_get_icon(m_GVolume);

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

QFuture<GVFSMount*> GVFSVolume::mount() {
    if (m_FutureMount != NULL)
        return m_FutureMount->future();

    m_FutureMount = new QFutureInterface<GVFSMount*>();

    GMountOperation *op;
    op = g_mount_operation_new();

    g_signal_connect(op, "ask_password", G_CALLBACK(handleAskPassword), NULL);

    g_volume_mount(m_GVolume,
            G_MOUNT_MOUNT_NONE,
            op,
            NULL,
            handleMountFinish,
            this);

    m_FutureMount->reportStarted();

    return m_FutureMount->future();
}

bool GVFSVolume::mountable() {
    return g_volume_can_mount(m_GVolume);
}

bool GVFSVolume::ejectable() {
    return g_volume_can_eject(m_GVolume);
}

bool GVFSVolume::automount() {
    return g_volume_should_automount(m_GVolume);
}

void GVFSVolume::handleAskPassword(GMountOperation* op, gchar* message, gchar* default_user, gchar* default_domain, GAskPasswordFlags flags, GVFSVolume * volume) {
    volume->m_FutureMount->setProgressValueAndText(-1, "Requesting autentication.");
    // TODO: Conect to the authentication service
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet";
}

void GVFSVolume::handleMountFinish(GObject* object, GAsyncResult* res, gpointer userdata) {
    GVFSVolume * volume = static_cast<GVFSVolume *> (userdata);
    if (!volume) {
        qWarning() << __PRETTY_FUNCTION__ <<  " unable to cast from gpointer";
        return;
    }
    GError *error = NULL;
    gboolean succeeded;
    GVolume * gvolume = G_VOLUME(object);

    succeeded = g_volume_mount_finish(gvolume, res, &error);

    if (!succeeded) {
        volume->m_FutureMount->setProgressValueAndText(-1, QString("Unable to mount %1, due %2").arg(g_volume_get_identifier(gvolume, G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE)).arg(error->message));
        volume->m_FutureMount->reportFinished();
        g_object_unref(error);
    } else {
        GMount *gmount;
        
        gmount = g_volume_get_mount(gvolume);
        volume->m_FutureMount->setProgressValueAndText(1, "Mounted.");
        GVFSMount * mount = new GVFSMount(gmount);
        volume->m_FutureMount->reportFinished(&mount);
        volume->m_FutureMount = NULL;
    }
}

QFuture<void> GVFSVolume::eject() {
    qDebug()<< __PRETTY_FUNCTION__ << "not implemented yet.";
}
