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

//! [Activator]

#include "CoreContext.h"
#include "IQWidget.h"

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>
#include <usServiceObjects.h>
#include <usLDAPProp.h>

#include <set>
#include <memory>

#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QApplication>

US_USE_NAMESPACE


class Activator : public ModuleActivator {

    /**
     * Implements ModuleActivator::Load(). 
     */
    void Load(ModuleContext* context) {
        QSettings settings(QApplication::applicationName(), CoreContext::getCoreContext()->getProperty("profile").toString());
        if (!QFile::exists(settings.fileName())) {
            qDebug() << "WARNING: The specified profile doesn't exist:\n\t" <<
                    settings.fileName() <<
                    "\n\tLoading core configuration UI.";
            //showCoreSettingsManager(profile);
            return;
        }
        qDebug() << "[    ] Loading toplevel widgets" << endl;

        int nwidgets = settings.beginReadArray("Widgets");
        if (nwidgets > 0) {
            for (int i = 0; i < nwidgets; i++) {
                settings.setArrayIndex(i);
                QString widgetName = settings.value("Class").toString();


                std::vector<us::ServiceReference<IQWidget> > refs =
                        context->GetServiceReferences<IQWidget>(us::LDAPProp("org.moonlightde.IQWidget.name") == widgetName.toStdString());
                if (refs.empty()) {
                    qDebug() << "ERROR: There aren't widgets registred under the name: " << widgetName <<
                            "\n\tMake shure you include the module in the profile.";
                   
                } else {
                    us::ServiceObjects<IQWidget> widgetObjects = context->GetServiceObjects(refs[0]);
                    // create a new instance
                    IQWidget* qWidget = widgetObjects.GetService();
                    qWidget->show();
                }
            }
            settings.endArray();
        } else {
            qDebug() << "ERROR: Sorry there are no configured widgets at " << settings.fileName() << endl;
        }
    }

    /**
     * Implements ModuleActivator::Unload(). Does nothing since
     * the C++ Micro Services library will automatically unregister any registered services.
     * @param context the context for the module.
     */
    void Unload(ModuleContext* /*context*/) {
        // NOTE: The service is automatically unregistered
    }

};

US_EXPORT_MODULE_ACTIVATOR(DesktopComposer, Activator)