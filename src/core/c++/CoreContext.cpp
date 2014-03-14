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

#include "CoreContext.h"


#include <QDebug>

CoreContext * CoreContext::m_coreContext = NULL;

QVariant CoreContext::getProperty(const QString &name) {
    return m_settings.value(name);
}

void CoreContext::setProperty(const QString &name, const QVariant &value) {
    m_settings.insert(name, value);
}

CoreContext * CoreContext::getCoreContext() {
    if (!m_coreContext)
        qDebug() << "WARING: core context not initialized.";
    return m_coreContext;
}

CoreContext::CoreContext(const QString &profile, const QString &pluginsDir) {
    m_settings.insert("profile", profile);
    m_settings.insert("pluginsDir", pluginsDir);
    
    m_moduleManager = new ModuleManager(profile, pluginsDir);

}

CoreContext::~CoreContext() {
}

void CoreContext::init(const QString &profile, const QString &pluginsDir) {
    //CoreContext *m_coreContext;
    if (!m_coreContext) {
        m_coreContext = new CoreContext(profile, pluginsDir);
    } else {
        qDebug() << "WARNING: CoreContext already initialized.";
    }

}

