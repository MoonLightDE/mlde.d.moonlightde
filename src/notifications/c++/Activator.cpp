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

#include "Feedbacker.h"


#include <LXQt/Application>

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>


#include <QDebug>
#include <QPointer>
#include <QtDBus/QDBusConnection>

US_USE_NAMESPACE

/*! \mainpage MoonLightDE notification daemon (adapted from LXDE-Qt)
 *
 * Running in user session; implementing standard as described in:
 *    docs/nodification-spec-latest.html
 *
 * <b>Implementation notes:</b>
 *
 * Class \c Notifyd implements the main "server" part, a DBUS
 * interface. Displaying of notifications is handled by these
 * classes:
 *
 *  - \c NotificationArea: a QScrollArea object with transparency.
 *       It ensures tha no action is unreachable (user can scroll
 *       over notifications)
 *  - \c NotificationLayout: a \c NotificationArea's main widget,
 *       (QWidget instance) holding instances of \c Notification.
 *       Layouting (in real QLayout) is done here.
 *  - \c Notification: a QWidget with one notification. Icon, texts,
 *       user interaction, etc. is handled in it.
 *
 * \c Notification can be extended with widgets located in files
 * notificationwidgets.*. Currently there is only one extension:
 *
 *  - \c NotificationActionsWidget holding user interface for
 *       interactive actions (buttons or combobox).
 *
 * Other extensions for e.g. "x-cannonical-*" can be implemented too.
 *
 */

#include "Feedbacker.h"

class Activator : public ModuleActivator {
private:

    /**
     * Implements ModuleActivator::Load().
     *
     * @param context the framework context for the module.
     */
    void Load(ModuleContext* context) {
        
        feedbacker = new Feedbacker();
        //        qDebug() << "Service loaded";
        //        daemon = new Notifyd();
        //        dbus_adaptor = new NotificationsAdaptor(daemon);
        //
        //        QDBusConnection connection = QDBusConnection::sessionBus();
        //        if (!connection.registerService("org.freedesktop.Notifications"))
        //            qDebug() << "registerService failed: another service with 'org.freedesktop.Notifications' runs already";
        //        if (!connection.registerObject("/org/freedesktop/Notifications", daemon))
        //            qDebug() << "registerObject failed: another object with '/org/freedesktop/Notifications' runs already";


        //        ServiceProperties props;
        //        context->RegisterService<INotificationsService>(daemon, props);
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
        delete feedbacker;
        //        delete dbus_adaptor;
        //        delete daemon;
    }

    //    QPointer<Notifyd> daemon;
    //    QPointer<NotificationsAdaptor> dbus_adaptor;
    QPointer<Feedbacker> feedbacker;
};
US_EXPORT_MODULE_ACTIVATOR(notifications, Activator)
