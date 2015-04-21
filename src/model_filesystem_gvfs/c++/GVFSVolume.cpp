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
#include "EjectVolumeOp.h"
#include <QFutureInterface>
#include <glib-2.0/gio/gvolume.h>

GVFSVolume::GVFSVolume(GVolume * gvolume) : m_GVolume(gvolume) {
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

GVFSMount* GVFSVolume::getMount() {
    GMount * gmount = g_volume_get_mount(m_GVolume);
    if (gmount != NULL)
        return new GVFSMount(gmount);

    return NULL;
}

QFuture<GVFSMount*> GVFSVolume::mount() {
    MountVolumeOp *op = new MountVolumeOp(m_GVolume);
    return op->run();
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

QFuture<void> GVFSVolume::eject() {
    EjectVolumeOp *op = new EjectVolumeOp(m_GVolume);
    return op->run();
}
