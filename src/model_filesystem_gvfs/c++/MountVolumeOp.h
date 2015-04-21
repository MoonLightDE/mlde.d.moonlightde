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

#ifndef MOUNTVOLUMEOP_H
#define	MOUNTVOLUMEOP_H


#include "GVFSVolume.h"
#include "GVFSMount.h"

#include <gio/gio.h>

#include <QTimer>
#include <QFutureInterface>


class MountVolumeOp : public QObject {
    Q_OBJECT
public:
    MountVolumeOp(GVolume * gvolume);
    virtual ~MountVolumeOp();

    virtual QFuture<GVFSMount*> run();

private:
    static void handleAskPassword(GMountOperation *op, gchar *message, gchar *default_user, gchar *default_domain, GAskPasswordFlags flags, gpointer userdata);
    static void handleFinish(GObject *object, GAsyncResult *res, gpointer userdata);

    GVolume * m_GVolume;
    QFutureInterface<GVFSMount*> m_futureInterface;
};

#endif	/* MOUNTVOLUMEOP_H */

