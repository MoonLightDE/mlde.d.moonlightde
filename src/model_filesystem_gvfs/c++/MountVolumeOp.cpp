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

#include "MountVolumeOp.h"
#include "module_config.h"

#include <QDebug>

MountVolumeOp::MountVolumeOp(GVolume * gvolume) : QObject(), m_GVolume(gvolume) {
    G_VOLUME(m_GVolume);
}

MountVolumeOp::~MountVolumeOp() {
    qDebug() << MODULE_NAME_STR << " mount volume operation deleted";
}

QFuture<GVFSMount*> MountVolumeOp::run() {
    if (G_VOLUME(m_GVolume)) {
        GMountOperation *op;
        op = g_mount_operation_new();

        g_signal_connect(op, "ask_password", G_CALLBACK(handleAskPassword), this);

        m_futureInterface.reportStarted();
        QString statusMsg = "Mounting ";
        char * cname = g_volume_get_name(m_GVolume);
        statusMsg += cname;
        g_free(cname);
        m_futureInterface.setProgressValueAndText(0, statusMsg);

        g_volume_mount(m_GVolume,
                G_MOUNT_MOUNT_NONE,
                op,
                NULL,
                handleFinish,
                this);

        g_object_unref(op);
    } else
        m_futureInterface.reportFinished(NULL);
    
    return m_futureInterface.future();
}

void MountVolumeOp::handleAskPassword(GMountOperation* op, gchar* message, gchar* default_user, gchar* default_domain, GAskPasswordFlags flags, gpointer userdata) {
    MountVolumeOp * mountVolume = static_cast<MountVolumeOp *> (userdata);
    if (!mountVolume) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }
    mountVolume->m_futureInterface.setProgressValueAndText(0, "Waithing for user credentials ");
    // TODO: Conect to the authentication service
    qDebug() << __PRETTY_FUNCTION__ << " not implemented yet";

    g_mount_operation_reply(op, G_MOUNT_OPERATION_HANDLED);
}

void MountVolumeOp::handleFinish(GObject* object, GAsyncResult* res, gpointer userdata) {
    MountVolumeOp * mountVolume = static_cast<MountVolumeOp *> (userdata);
    if (!mountVolume) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }

    GError *error = NULL;
    gboolean succeeded;
    GVolume * gvolume = G_VOLUME(object);

    succeeded = g_volume_mount_finish(gvolume, res, &error);

    if (!succeeded) {
        QString errorMsg = "Unable to mount %1, due %2";

        char * cname = g_volume_get_name(mountVolume->m_GVolume);
        errorMsg = errorMsg.arg(cname);
        g_free(cname);

        errorMsg = errorMsg.arg(error->message);

        mountVolume->m_futureInterface.setProgressValueAndText(1, errorMsg);
        mountVolume->m_futureInterface.reportFinished(NULL);
        g_error_free(error);
    } else {
        GMount *gmount;

        gmount = g_volume_get_mount(gvolume);
        GVFSMount * mount = new GVFSMount(gmount);
        mountVolume->m_futureInterface.reportFinished(&mount);
    }

    delete mountVolume;
}
