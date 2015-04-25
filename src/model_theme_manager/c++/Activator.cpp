/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * 
 * Authors:
 *  Alexis López Zubieta
 * 
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

#include "ThemeManager.h"
#include "core/ICore.h"
#include "core/ModuleSettings.h"

#include <usModule.h>
#include <usModuleEvent.h>
#include <usModuleContext.h>
#include <usServiceTracker.h>
#include <usModuleActivator.h>
#include <usServiceReference.h>


#include <QDebug>
#include <QPointer>

US_USE_NAMESPACE
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
        QSettings *settings = ModuleSettings::getModuleSettings(context);
        QString themeName = settings->value("ThemeName", "").toString();
        QString iconsThemeName = settings->value("IconsThemeName", "" ).toString();
        factory.setQssTheme(themeName);
        factory.setIconTheme(iconsThemeName);

        delete settings;
        // TODO: Define qss theme
        // TODO: Define icons theme

        try {
            // Get active modules
            ServiceReference<Core::IModuleManager> ref =
                    context->GetServiceReference<Core::IModuleManager>();

            Core::IModuleManager *moduleManager = context->GetService<Core::IModuleManager>(ref);
            QList<QString> modules = moduleManager->getActiveModules();

            // Load modules qss 

            foreach(QString module, modules) {
                factory.loadModuleQSS(module);
            }
        } catch (ServiceException ex) {
            qDebug() << "Unable to find IModuleManager";
        } catch (std::logic_error ex) {
            qDebug() << "Unable to find IModuleManager";
        }
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.QPointer<
     */
    void Unload(ModuleContext* context) {
    }

    void moduleEventListener(const ModuleEvent event) {
        if (event.GetType() == ModuleEvent::LOADED) {
            Module * module = event.GetModule();
            if (module != NULL) {
                //                qDebug() << "Loding theme for : " <<module->GetName();

            }
        }
    }
    ThemeManager factory;
};
US_EXPORT_MODULE_ACTIVATOR(model_theme_manager, Activator)