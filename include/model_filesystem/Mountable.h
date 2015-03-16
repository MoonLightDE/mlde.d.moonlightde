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

#ifndef MOUNTABLE_H
#define	MOUNTABLE_H

#include <QObject>

namespace model_filesystem {

    /**
     * Used to describe and manage nodes that require especial mount & umount 
     * operations, like smb shares, etc.
     */
    class Mountable {
    public:
        virtual void mount() = 0;
        virtual void unmount() = 0;
        virtual void eject() = 0;

    signals:
        void mounted();
        void unmounted();
        void ejected();

    };

    Q_DECLARE_INTERFACE(Mountable, "org.moonlightde.model_filesystem.Mountable/1.0")
}
#endif	/* MOUNTABLE_H */

