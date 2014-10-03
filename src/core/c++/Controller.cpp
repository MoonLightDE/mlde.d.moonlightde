/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
 * 
 * This file is part of Moonlight Desktop Environment Core.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Controller.h"
#include "ModuleManager.h"
#include "core/IQt5.h"
#include "core/ModuleSettings.h"

#include <usGetModuleContext.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

#include <QDebug>
#include <QApplication>
#include <QFile>

using namespace us;
using namespace Core;

Controller::Controller(const QHash<QString, QVariant> &config) {
    ModuleContext * context = GetModuleContext();

    ModuleSettings::setProfileName(config["profile"].toString());
    m_profile = ModuleSettings::getModuleSettings(context);

    moduleManager = new ModuleManager(config);
    {
        ServiceProperties props;
        context->RegisterService<Core::IModuleManager>(moduleManager, props);
    }
    {
        ServiceProperties props;
        props["Module"] = std::string("Core");
        context->RegisterService<QSettings>(m_profile, props);
    }
    {
        ServiceProperties props;
        context->RegisterService<Core::IController>(this, props);
    }
    if (config["showConfig"].toBool()) {
        m_configUi = new CoreConfigurationUI();
        m_configUi->show();
    } else {
        m_configUi = NULL;

    }
}

void Controller::start() {
    // Loading modules
    const QStringList list = moduleManager->getStartUpModules();

    foreach(QString module, list) {
        moduleManager->load(module);
    }
    emit(started());

    if (moduleManager->getActiveModules().size() <= 1) {
        if (m_configUi == NULL)
            m_configUi = new CoreConfigurationUI();
        m_configUi->show();
    }
}

void Controller::finish() {
    qApp->exit();
    emit(finishing());
}

Controller::~Controller() {
    delete m_profile;
}

