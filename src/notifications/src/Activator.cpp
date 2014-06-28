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

#include <QApplication>
#include "lxqttranslate.h"
#include "notificationsadaptor.h"
#include "notifyd.h"

#include <LXQt/Application>

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>


#include <QDebug>
#include <QPointer>
#include <QtDBus/QDBusConnection>

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
        qDebug() << "Service loaded";
        daemon = new Notifyd();
        dbus_adaptor = new NotificationsAdaptor(daemon);

        QDBusConnection connection = QDBusConnection::sessionBus();
        if (!connection.registerService("org.freedesktop.Notifications"))
            qDebug() << "registerService failed: another service with 'org.freedesktop.Notifications' runs already";
        if (!connection.registerObject("/org/freedesktop/Notifications", daemon))
            qDebug() << "registerObject failed: another object with '/org/freedesktop/Notifications' runs already";


//        ServiceProperties props;
//        context->RegisterService<IPanel>(m_panel, props);
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
        delete dbus_adaptor;
        delete daemon;
    }

    QPointer<Notifyd> daemon;
    QPointer<NotificationsAdaptor> dbus_adaptor;
};
US_EXPORT_MODULE_ACTIVATOR(Notifications_Service, Activator)
