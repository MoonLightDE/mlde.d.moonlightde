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

#ifndef GVFSVOLUME_H
#define	GVFSVOLUME_H

#include <gio/gio.h>

#include <QFuture>
#include <QThread>

class GVFSMount;

class GVFSVolume {
public:
    GVFSVolume(GVolume * gvolume);
    virtual ~GVFSVolume();

    virtual QString name();
    virtual QString uuid();
    virtual QString iconName();

    virtual GVFSMount * getMount();
    virtual QFuture<GVFSMount*> mount();
    virtual QFuture<void> eject();

    virtual bool mountable();
    virtual bool ejectable();
    virtual bool automount();

private:
    GVolume * m_GVolume;
};

#endif	/* GVFSVOLUME_H */

