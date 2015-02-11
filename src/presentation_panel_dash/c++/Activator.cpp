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


#include "Dash.h"
#include "presentation_panel/WidgetFactory.h"

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

#include <QObject>
#include <QPushButton>
#include <QDebug>

US_USE_NAMESPACE
/**
 */
class Activator : public ModuleActivator, public presentation_panel::WidgetFactory {
private:

    virtual QWidget* build(const QString& prefix, QWidget* parent) {
        QPushButton *bttn = new QPushButton("&Start", parent);
        bttn->setObjectName(presentation_panel::MAINMENUBUTTON);

        QObject::connect(bttn, SIGNAL(clicked()), &m_Dash, SLOT(show()));

        return bttn;
    }

    virtual QString name() {
        return presentation_panel::MAINMENUBUTTON;
    }

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
    }

    Dash m_Dash;
};
US_EXPORT_MODULE_ACTIVATOR(presentation_panel_dash, Activator)