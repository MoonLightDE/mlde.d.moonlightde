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

GVFSMount::GVFSMount(GMount * gmount) : QObject() {
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
    if (G_VOLUME(gvolume))
        return new GVFSVolume(gvolume);
    return NULL;
}

QFuture<void> GVFSMount::unmount() {
    UnmountMountOp *umountOp = new UnmountMountOp(m_GMount);
    return umountOp->run();
}

bool GVFSMount::removable() {
    return g_mount_can_unmount(m_GMount);
}

bool GVFSMount::ejectable() {
    return g_mount_can_eject(m_GMount);
}