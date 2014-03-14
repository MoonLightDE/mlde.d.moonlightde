/*
 * Copyright (C) 2013 Moonlight Desktop Environment Team
 * Authors:
 * Alexis López Zubieta
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

#ifndef IMODULEMANAGER_H
#define	IMODULEMANAGER_H

#include <usModule.h>
#include <QString>

class IModuleManager {
public:
    virtual void loadModule(const QString &name, const bool &persistent = false) = 0;
    virtual void unloadModule(const QString &name, const bool &persistent = false) = 0;

private:

};

US_DECLARE_SERVICE_INTERFACE(IModuleManager, "org.moonlightDE.IModuleManager")

#endif	/* IMODULEMANAGER_H */

