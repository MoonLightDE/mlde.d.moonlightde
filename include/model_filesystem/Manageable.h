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


#ifndef MANAGEABLE_H
#define	MANAGEABLE_H
namespace model_filesystem {

    /**
     * Define the Unix classes where user privileges are applicable.
     */
    enum UNIX_CLASS {
        NONE = 0,
        OWNER,
        GROUP,
        ALL
    };

    /**
     * Used to describe and edit the access the permissions to <code>NODE</code>.
     */
    class Manageable {
    public:
        virtual QString owner() = 0;
        virtual bool setOwner(const QString &owner) = 0;

        virtual QString group() = 0;
        virtual bool setGroup(const QString &group) = 0;

        virtual bool canRead(UNIX_CLASS unixClass) = 0;
        virtual bool setCanRead(UNIX_CLASS unixClass, bool canRead) = 0;

        virtual bool canWrite(UNIX_CLASS unixClass) = 0;
        virtual bool setCanWrite(UNIX_CLASS unixClass, bool canWrite) = 0;

        virtual bool canExecute(UNIX_CLASS unixClass) = 0;
        virtual bool setCanExecute(UNIX_CLASS unixClass, bool canExecute) = 0;
    };
}
#endif	/* MANAGEABLE_H */

