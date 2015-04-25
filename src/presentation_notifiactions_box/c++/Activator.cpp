/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
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
#include "model_notifications/Stack.h"
#include "NotificationsController.h"

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

#include <QMutex>
#include <QMutexLocker>

#include <QPointer>
#include <QObject>
#include <QDebug>

US_USE_NAMESPACE
/**
 */
class Activator : public ModuleActivator {

    /**
     * Implements ServiceListener.serviceChanged(). Checks to see if the service
     * we are using is leaving or tries to get a service if we need one.
     *
     * @param event the fired service event.
     */
    void ServiceChanged(const ServiceEvent event) {
        QMutexLocker lock(&m_Mutex);

        // If a stack service was registered, see if we
        // need one. If so, get a reference to it.
        if (event.GetType() == ServiceEvent::REGISTERED) {
            if (!m_ServiceRef) {
                // Get a reference to the service object.
                m_ServiceRef = event.GetServiceReference();
                m_Controller.setStack(m_context->GetService(m_ServiceRef));
            }
        }// If a stack service was unregistered, see if it
            // was the one we were using. If so, unget the service
            // and try to query to get another one.
        else if (event.GetType() == ServiceEvent::UNREGISTERING) {
            if (event.GetServiceReference() == m_ServiceRef) {
                // Unget service object and null references.
                m_context->UngetService(m_ServiceRef);
                m_ServiceRef = 0;
                m_Controller.unsetStack();


                // Query to see if we can get another service.
                std::vector<ServiceReference<model_notifications::Stack> > refs;
                try {
                    refs = m_context->GetServiceReferences<model_notifications::Stack>("");
                } catch (const std::invalid_argument& e) {
                    std::cout << e.what() << std::endl;
                }
                if (!refs.empty()) {
                    // Get a reference to the first service object.
                    m_ServiceRef = refs.front();
                    m_Controller.setStack(m_context->GetService(m_ServiceRef));
                }
            }
        }
    }

private:

    /**
     * Implements ModuleActivator::Load().
     *
     * @param context the framework context for the module.
     */
    void Load(ModuleContext* context) {
        m_context = context;

        // we don't want to receive service events when looking up the
        // stack service, if one exists.
        QMutexLocker lock(&m_Mutex);

        // Listen for events pertaining to stack services.
        m_context->AddServiceListener(this, &Activator::ServiceChanged,
                std::string("(&(") + ServiceConstants::OBJECTCLASS() + "=" +
                us_service_interface_iid<model_notifications::Stack>() + "))");

        std::vector<ServiceReference<model_notifications::Stack> > refs =
                context->GetServiceReferences<model_notifications::Stack>("");
        // If we found any dictionary services, then just get
        // a reference to the first one so we can use it.
        if (!refs.empty()) {
            m_ServiceRef = refs.front();
            m_Controller.setStack(m_context->GetService(m_ServiceRef));
        }


    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
        qDebug() << MODULE_NAME << ": unloaded.";
    }
    QMutex m_Mutex;
    ModuleContext * m_context;

    ServiceReference<model_notifications::Stack> m_ServiceRef;

    NotificationsController m_Controller;
};
US_EXPORT_MODULE_ACTIVATOR(presentation_notifiactions_box, Activator)
