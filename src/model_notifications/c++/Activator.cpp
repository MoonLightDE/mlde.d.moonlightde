/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
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



#include "StackImpl.h"
#include "module_config.h"
#include "NotificationsDbusInterface.h"
#include "notificationsadaptor.h"


#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

#include <QDBusConnection>
#include <QDebug>

US_USE_NAMESPACE


class Activator : public ModuleActivator {
private:

    /**
     * Implements ModuleActivator::Load().
     *
     * @param context the framework context for the module.
     */
    void Load(ModuleContext* context) {
        m_DBusInterface = new NotificationsDbusInterface(&m_NotificationsStack);
        m_NotificationsAdaptor = new NotificationsAdaptor(m_DBusInterface);

        // Publish dbus service
        QDBusConnection connection = QDBusConnection::sessionBus();
        if (!connection.registerService("org.freedesktop.Notifications"))
            qDebug() << "registerService failed: another service with 'org.freedesktop.Notifications' runs already";
        if (!connection.registerObject("/org/freedesktop/Notifications", m_DBusInterface))
            qDebug() << "registerObject failed: another object with '/org/freedesktop/Notifications' runs already";

        // Publish local service
        context->RegisterService<model_notifications::Stack>(&m_NotificationsStack, ServiceProperties());
        
//        qDebug() << MODULE_NAME << ": LOAD FINISHED";
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
        delete m_NotificationsAdaptor;
        delete m_DBusInterface;
//        qDebug() << MODULE_NAME << ": UNLOAD FINISHED";
    }
    StackImpl m_NotificationsStack;
    NotificationsDbusInterface *m_DBusInterface;
    NotificationsAdaptor* m_NotificationsAdaptor;

};
US_EXPORT_MODULE_ACTIVATOR(model_notifications, Activator)
