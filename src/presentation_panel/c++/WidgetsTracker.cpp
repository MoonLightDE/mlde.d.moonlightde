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



#include "WidgetsTracker.h"
#include "PanelImpl.h"
#include "module_config.h"

#include <usModuleContext.h>
#include <usGetModuleContext.h>

#include <QDebug>

WidgetsTracker::WidgetsTracker(PanelImpl *panel) {
    m_Context = us::GetModuleContext();

    if (panel == NULL)
        qCritical() << MODULE_NAME << ": invalid widget service tracker initialization.";
    else {
        m_Panel = panel;
        m_tracker = new us::ServiceTracker<presentation_panel::WidgetFactory>(m_Context, this);
        m_tracker->Open();

        qDebug() << MODULE_NAME << ": panel widgets tracker up and running.";
    }
}

WidgetsTracker::~WidgetsTracker() {
    m_tracker->Close();
}

presentation_panel::WidgetFactory* WidgetsTracker::AddingService(const us::ServiceReference<presentation_panel::WidgetFactory>& reference) {
    presentation_panel::WidgetFactory * factory = m_Context->GetService(reference);

    if (m_Panel.isNull()) {
        qCritical() << MODULE_NAME << ": widget service tracker not initialized.";
        return factory;
    }

    m_Panel->addWidgetFactory(factory);
    return factory;
}

void WidgetsTracker::ModifiedService(const us::ServiceReference<presentation_panel::WidgetFactory>& reference, presentation_panel::WidgetFactory *service) {
    // do nothing
}

void WidgetsTracker::RemovedService(const us::ServiceReference<presentation_panel::WidgetFactory>& reference, presentation_panel::WidgetFactory *service) {
    if (m_Panel.isNull()) {
        qCritical() << MODULE_NAME << ": widget service tracker not initialized.";
        return;
    }
    m_Panel->removeWidgetFactory(service);
}
