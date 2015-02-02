/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
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
#include "WidgetsTracker.h"

#include <usModuleContext.h>
#include <usGetModuleContext.h>

#include <QDebug>

WidgetsTracker::WidgetsTracker(us::ModuleContext* context, PanelImpl *panel) {
    m_Context = context;
    
    if (panel == NULL)
        qCritical() << MODULE_NAME  << ": invalid widget service tracker initialization.";
    else {
        m_Panel = panel;
        m_tracker = new us::ServiceTracker<presentation_panel::Widget>(m_Context, this);
        m_tracker->Open();
        
        qDebug() << MODULE_NAME << ": panel widgets tracker up and running.";
    }
}

WidgetsTracker::~WidgetsTracker() {
    m_tracker->Close();
}

presentation_panel::Widget* WidgetsTracker::AddingService(const us::ServiceReference<presentation_panel::Widget>& reference) {
    presentation_panel::Widget * widget = m_Context->GetService(reference);

    if (m_Panel == NULL) {
        qCritical() << MODULE_NAME << ": widget service tracker not initialized.";
        return widget;
    }

    m_Panel->addWidget(widget);
}

void WidgetsTracker::ModifiedService(const us::ServiceReference<presentation_panel::Widget>&, presentation_panel::Widget *widget) {
    // do nothing
}

void WidgetsTracker::RemovedService(const us::ServiceReference<presentation_panel::Widget>& reference, presentation_panel::Widget *widget) {
    if (m_Panel == NULL) {
        qCritical() << MODULE_NAME << ": widget service tracker not initialized.";
        return;
    }
    m_Panel->removeWidget(widget);
}
