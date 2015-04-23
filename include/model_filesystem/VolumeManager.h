/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
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

#ifndef VOLUMEMANAGER_H
#define	VOLUMEMANAGER_H

#include <usServiceInterface.h>

#include <QList>
#include <QString>
#include <QObject>

namespace model_filesystem {

    class Volume;
    class Mount;

    class VolumeManager : public QObject {

        Q_OBJECT
    public:

        virtual ~VolumeManager() {
        };

        virtual QList<Volume *> volumes() = 0;
        virtual QList<Mount *> mounts() = 0;

Q_SIGNALS:
        void mountAdded(QString name);
        void mountRemoved(QString name);
        //void mount_changed(QString name);
        void mountPremount(QString name);

        void volumeAdded(QString name);
        void volumeRemoved(QString name);
    private:

    };
}

Q_DECLARE_INTERFACE(model_filesystem::VolumeManager, "org.moonlightde.panel.model_filesystem.VolumeManager/1.0")
US_DECLARE_SERVICE_INTERFACE(model_filesystem::VolumeManager, "org.moonlightde.panel.model_filesystem.VolumeManager/1.0")
#endif	/* VOLUMEMANAGER_H */

