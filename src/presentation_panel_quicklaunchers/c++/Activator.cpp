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

#include "module_config.h"

#include <core/ICore.h> 

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

#include "lxqtquicklaunch.h"
#include <QPointer>
#include <QDebug>
#include "presentation_panel/WidgetFactory.h"

US_USE_NAMESPACE
/**
 */
class Activator : public ModuleActivator, public presentation_panel::WidgetFactory  {
public:
    virtual QWidget* build(const QString& prefix, QWidget* parent) {
        if (m_quicklaunchers.isNull()) {
            m_quicklaunchers = new LxQtQuickLaunch(parent);
            m_quicklaunchers->setObjectName(presentation_panel::QUICKLAUNCHS);
        }
        return m_quicklaunchers;
    }
    virtual QString name() {
        return presentation_panel::QUICKLAUNCHS;
    }

private:

    /**
     * Implements ModuleActivator::Load().
     *
     * @param context the framework context for the module.
     */
    void Load(ModuleContext* context) {
        ServiceProperties props;
        context->RegisterService<presentation_panel::WidgetFactory>(this, props);
        qDebug() << MODULE_NAME << ": loaded.";
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
        delete m_quicklaunchers;
        qDebug() << MODULE_NAME << ": unloaded.";
    }
    
    QPointer<LxQtQuickLaunch> m_quicklaunchers;

};
US_EXPORT_MODULE_ACTIVATOR(presentation_panel_quicklaunchers, Activator)
