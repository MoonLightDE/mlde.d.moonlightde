/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
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


#include <QCoreApplication>

#include <QString>
#include <QDebug>
#include <QPointer>
#include <QStringList>

#include "meta_types.h"
#include "core.h"


#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

US_USE_NAMESPACE

#define DEFAULT_CONFIG ".config/LXQt/globalkeyshortcuts.conf"

/**
 */
class Activator : public ModuleActivator {
private:

    /**
     * Implements ModuleActivator::Load().
     *
     * @param context the framework context for the module.
     */
    void Load(ModuleContext* context) {
        qDebug() << "Shortcuts-Service Startting";

        bool wrongArgs = false;
        bool printHelp = false;
        bool runAsDaemon = true;
        bool useSyslog = false;
        bool minLogLevelSet = false;
        int minLogLevel = LOG_NOTICE;
        bool multipleActionsBehaviourSet = false;
        MultipleActionsBehaviour multipleActionsBehaviour = MULTIPLE_ACTIONS_BEHAVIOUR_FIRST;
        QStringList configFiles;

        configFiles.push_back(QString::fromLocal8Bit(getenv("HOME")) + "/" DEFAULT_CONFIG);

        m_core = new Core(runAsDaemon || useSyslog, minLogLevelSet, minLogLevel, configFiles, multipleActionsBehaviourSet, multipleActionsBehaviour);

        if (!m_core->ready()) {
            qCritical() << "Unable to start Shortcuts-Service";
            return;
        }
        
        
//        m_panel = new Panel();
//
//        ServiceProperties props;
//        context->RegisterService<IPanel>(m_panel, props);
//
//        m_panel.data()->show();
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
    }

        QPointer<Core> m_core;
};
US_EXPORT_MODULE_ACTIVATOR(shortcuts_services, Activator)
