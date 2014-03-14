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

#ifndef CORECONTEXT_H
#define	CORECONTEXT_H

#include "ModuleManager.h"

#include <QString>
#include <QVariant>

class CoreContext {
public:
    static void init(const QString &profile, const QString &pluginsDir);
    static CoreContext * getCoreContext();

    QVariant getProperty(const QString &name);
    void setProperty(const QString &name, const QVariant &value);


private:
    static CoreContext * m_coreContext;
    ModuleManager *m_moduleManager;

    CoreContext(const QString &profile, const QString &pluginsDir);
    virtual ~CoreContext();

    QVariantHash m_settings;



};

#endif	/* CORECONTEXT_H */

