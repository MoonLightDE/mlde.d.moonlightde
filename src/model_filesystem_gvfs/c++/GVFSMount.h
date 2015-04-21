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

#ifndef GVFSMOUNT_H
#define	GVFSMOUNT_H
#include <gio/gio.h>

class GVFSVolume;
class GVFSDirectory;

#include "GVFSVolume.h"
#include "MountVolumeOp.h"

#include <QObject>
#include <QFuture>
#include <QString>

class GVFSMount : public QObject {
    Q_OBJECT
    friend class MountVolumeOp;
public:
    /**
     * Takes ownership over the GMount object.
     * @param gmount
     */
    GVFSMount(GMount * gmount);
    virtual ~GVFSMount();

    virtual QString name();
    virtual QString uuid();
    virtual QString iconName();

    virtual GVFSDirectory* root();
    virtual GVFSVolume* volume();

    virtual QFuture<void> unmount();


    virtual bool removable();
    virtual bool ejectable();

    
private:
    GMount * m_GMount;
};

#endif	/* GVFSMOUNT_H */

