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

#include "UnmountMountOp.h"

#include <QDebug>

UnmountMountOp::UnmountMountOp(GMount * gmount) : m_GMount(gmount) {
    G_MOUNT(m_GMount);
}

UnmountMountOp::~UnmountMountOp() {
    g_object_unref(m_MountOp);
    g_object_unref(m_GMount);
}

QFuture<void> UnmountMountOp::run() {
    m_MountOp = g_mount_operation_new();

    g_mount_unmount_with_operation(m_GMount, G_MOUNT_UNMOUNT_NONE, m_MountOp, NULL, handleFinish, this);

    m_FutureInterface.reportStarted();

    return m_FutureInterface.future();
}

void UnmountMountOp::handleFinish(GObject* object, GAsyncResult* res, gpointer userdata) {
    gboolean succeeded;
    GError *error = NULL;

    UnmountMountOp * unmountMount = static_cast<UnmountMountOp *> (userdata);
    if (!unmountMount) {
        qWarning() << __PRETTY_FUNCTION__ << " unable to cast from gpointer";
        return;
    }

    succeeded = g_mount_unmount_with_operation_finish(G_MOUNT(object), res, &error);


    if (!succeeded) {
        QString errorMsg = "Error unmounting mount: %s";
        errorMsg = errorMsg.arg(error->message);
        unmountMount->m_FutureInterface.setProgressValueAndText(-1, errorMsg);
        unmountMount->m_FutureInterface.reportFinished(NULL);

        g_error_free(error);
    } else {
        unmountMount->m_FutureInterface.setProgressValue(1);
        unmountMount->m_FutureInterface.reportFinished(NULL);
    }

    delete unmountMount;
}
