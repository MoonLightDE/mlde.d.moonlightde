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


#ifndef VOLUME_H
#define	VOLUME_H

#include <usServiceInterface.h>

#include <QString>
#include <QFuture>

namespace model_filesystem {
    class Mount;

    class Volume {
    public:

        virtual ~Volume() {
        };
        virtual QString name() = 0;
        virtual QString uuid() = 0;
        virtual QString iconName() = 0;

        virtual Mount * getMount() = 0;
        virtual QFuture<Mount*> mount() = 0;
        virtual QFuture<void> eject() = 0;

        virtual bool mountable() = 0;
        virtual bool ejectable() = 0;
        virtual bool automount() = 0;

    };
}
#endif	/* VOLUME_H */

