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

#include "lxqtclock.h"
#include "lxqtclockconfiguration.h"

//#include "panel/IPanel.h"
#include "presentation_panel/WidgetFactory.h"
#include "presentation_panel/Panel.h"

#include "core/ModuleSettings.h"

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>
#include <usGetModuleContext.h>

#include <QDebug>
#include <QPointer>

US_USE_NAMESPACE
/**
 */
class Activator : public ModuleActivator, public presentation_panel::WidgetFactory {
public:

    virtual QWidget* build(const QString& widgetName, presentation_panel::Panel* parent) {
        if (m_clock.isNull()) {
            m_clock = new LxQtClock(widgetName);
            m_clock->setObjectName(presentation_panel::DATETIME);
        }
        return m_clock->widget();
    }

    virtual QString name() {
        return presentation_panel::DATETIME;
    }

    virtual bool isCustomizable() {
        return true;
    }

    virtual QWidget* custimizationWidget(const QString& widgetName) {
        return m_clock->configureDialog();
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
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
        delete m_clock;
    }

    QPointer<LxQtClock> m_clock;
};
US_EXPORT_MODULE_ACTIVATOR(presentation_panel_datetime, Activator)
