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


#ifndef MOUNT_H
#define	MOUNT_H
#include <usServiceInterface.h>

#include <QFuture>
#include <QString>

namespace model_filesystem {
    class Directory;
    class Volume;

    class Mount {
    public:

        virtual ~Mount() {
        };

        virtual QString name() = 0;
        virtual QString uuid() = 0;
        virtual QString iconName() = 0;

        virtual Directory* root() = 0;
        virtual Volume* volume() = 0;

        virtual QFuture<void> unmount() = 0;


        virtual bool removable() = 0;
        virtual bool ejectable() = 0;
    };
}
#endif	/* MOUNT_H */

