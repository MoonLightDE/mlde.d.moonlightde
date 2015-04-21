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

#include "EjectVolumeOp.h"

EjectVolumeOp::EjectVolumeOp(GVolume * gvolume) : m_GVolume(gvolume) {
    G_VOLUME(m_GVolume);
}

EjectVolumeOp::~EjectVolumeOp() {
    g_object_unref(m_MountOp);
    g_object_unref(m_GVolume);
}

QFuture<void> EjectVolumeOp::run() {
    m_MountOp = g_mount_operation_new();
    g_volume_eject_with_operation(m_GVolume, G_MOUNT_UNMOUNT_NONE, m_MountOp, NULL, handleFinish, this);


    m_FutureInterface.reportStarted();

    return m_FutureInterface.future();
}

void EjectVolumeOp::handleFinish(GObject* object, GAsyncResult* res, gpointer userdata) {
    EjectVolumeOp * ejectVolumeOp = static_cast<EjectVolumeOp *> (userdata);
    if (!ejectVolumeOp) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }

    gboolean succeeded;
    GError *error = NULL;

    succeeded = g_volume_eject_with_operation_finish(G_VOLUME(object), res, &error);

    if (!succeeded) {
        QString errorMsg = "Error ejecting volume: %s";
        errorMsg = errorMsg.arg(error->message);
        ejectVolumeOp->m_FutureInterface.setProgressValueAndText(-1, errorMsg);
        ejectVolumeOp->m_FutureInterface.reportFinished(NULL);

        g_error_free(error);
    } else {
        ejectVolumeOp->m_FutureInterface.setProgressValue(1);
        ejectVolumeOp->m_FutureInterface.reportFinished(NULL);
    }

    delete ejectVolumeOp;
}

