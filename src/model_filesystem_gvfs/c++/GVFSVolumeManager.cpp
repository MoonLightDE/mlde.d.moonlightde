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
#include "GVFSVolumeManager.h"


#include <glib-object.h>

#include <QDebug>

#include <functional>

GVFSVolumeManager::GVFSVolumeManager() : QObject() {
    m_VolumeMonitor = g_volume_monitor_get();
    if (G_VOLUME_MONITOR(m_VolumeMonitor)) {
        g_signal_connect(m_VolumeMonitor, "mount-added", (GCallback) on_g_mount_added, this);
        g_signal_connect(m_VolumeMonitor, "mount-removed", (GCallback) on_g_mount_removed, this);
        g_signal_connect(m_VolumeMonitor, "mount-pre-unmount", (GCallback) on_g_mount_premount, this);
        //    g_signal_connect(volume_monitor, "mount-changed", (GCallback) monitor_mount_changed, NULL);

        g_signal_connect(m_VolumeMonitor, "volume-added", (GCallback) on_g_volume_added, this);
        g_signal_connect(m_VolumeMonitor, "volume-removed", (GCallback) on_g_volume_removed, this);

        //    g_signal_connect(volume_monitor, "volume-changed", (GCallback) monitor_volume_changed, NULL);
        //    g_signal_connect(volume_monitor, "drive-connected", (GCallback) monitor_drive_connected, NULL);
        //    g_signal_connect(volume_monitor, "drive-disconnected", (GCallback) monitor_drive_disconnected, NULL);
        //    g_signal_connect(volume_monitor, "drive-changed", (GCallback) monitor_drive_changed, NULL);
        //    g_signal_connect(volume_monitor, "drive-eject-button", (GCallback) monitor_drive_eject_button, NULL);
    } else {
        qWarning() << MODULE_NAME_STR << " unable to get a G Volume Monitor instance.";
    }
}

GVFSVolumeManager::~GVFSVolumeManager() {
    g_object_unref(m_VolumeMonitor);
}

QList<GVFSVolume*> GVFSVolumeManager::volumes() {
    QList<GVFSVolume*> list;
    if (G_VOLUME_MONITOR(m_VolumeMonitor)) {
        // Get existent volumes
        GList* gvolumes = g_volume_monitor_get_volumes(m_VolumeMonitor);
        GList* gvolumes_ptr = gvolumes;
        while (gvolumes_ptr != NULL) {
            if (G_VOLUME(gvolumes_ptr->data)) {
                GVolume * gvolume = G_VOLUME(gvolumes_ptr->data);
                GVFSVolume* volume = new GVFSVolume(gvolume);
                list.append(volume);
            }

            gvolumes_ptr = gvolumes_ptr->next;
        }
        g_list_free(gvolumes);

    } else {
        qWarning() << MODULE_NAME_STR << " unable to get a G Volume Monitor instance.";
    }
    return list;
}

QList<GVFSMount*> GVFSVolumeManager::mounts() {
    QList<GVFSMount*> list;
    if (G_VOLUME_MONITOR(m_VolumeMonitor)) {
        // Get existent mounts
        GList* gmounts = g_volume_monitor_get_mounts(m_VolumeMonitor);
        GList* gmounts_ptr = gmounts;
        qDebug() << MODULE_NAME_STR << "existent mounts:";
        while (gmounts_ptr != NULL) {
            if (G_MOUNT(gmounts_ptr->data)) {
                GMount * gmount = G_MOUNT(gmounts_ptr->data);
                GVFSMount *mount = new GVFSMount(gmount);
                list.append(mount);
            }

            gmounts_ptr = gmounts_ptr->next;
        }
        g_list_free(gmounts);
    } else {
        qWarning() << MODULE_NAME_STR << " unable to get a G Volume Monitor instance.";
    }
    return list;
}

void GVFSVolumeManager::handle_mount_added(GMount* mount) {
    gchar * gmountName = g_mount_get_name(mount);

    Q_EMIT(mountAdded(QString(gmountName)));

    qDebug() << MODULE_NAME_STR << gmountName << " added";
    g_free(gmountName);
}

void GVFSVolumeManager::on_g_mount_added(GVolumeMonitor* volume_monitor, GMount* mount, GVFSVolumeManager* volumeManager) {
    if (G_MOUNT(mount))
        volumeManager->handle_mount_added(mount);
}

void GVFSVolumeManager::handle_mount_removed(GMount* gmount) {
    qDebug() << MODULE_NAME_STR << g_mount_get_name(gmount) << " removed";
}

void GVFSVolumeManager::on_g_mount_removed(GVolumeMonitor* volume_monitor, GMount* mount, GVFSVolumeManager* volumeManager) {
    if (G_MOUNT(mount))
        volumeManager->handle_mount_removed(mount);
}

void GVFSVolumeManager::handle_mount_preunmount(GMount* gmount) {
    qDebug() << MODULE_NAME_STR << g_mount_get_name(gmount) << " preunmount";
}

void GVFSVolumeManager::on_g_mount_premount(GVolumeMonitor* volume_monitor, GMount* mount, GVFSVolumeManager* volumeManager) {
    if (G_MOUNT(mount))
        volumeManager->handle_mount_preunmount(mount);
}

void GVFSVolumeManager::handle_volume_added(GVolume* volume) {
    qDebug() << MODULE_NAME_STR << g_volume_get_name(volume) << " added";
}

void GVFSVolumeManager::on_g_volume_added(GVolumeMonitor* volume_monitor, GVolume* volume, GVFSVolumeManager* volumeManager) {
    if (G_VOLUME(volume))
        volumeManager->handle_volume_added(volume);
}

void GVFSVolumeManager::handle_volume_removed(GVolume* volume) {
    qDebug() << MODULE_NAME_STR << g_volume_get_name(volume) << " removed";
}

void GVFSVolumeManager::on_g_volume_removed(GVolumeMonitor* volume_monitor, GVolume* volume, GVFSVolumeManager* volumeManager) {
    if (G_VOLUME(volume))
        volumeManager->handle_volume_removed(volume);
}



