/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
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

#ifndef NOTIFICATIONSSTACK_H
#define	NOTIFICATIONSSTACK_H

#include <usServiceInterface.h>

#include <QVariant>
#include <QStack>
#include <QObject>


namespace model_notifications {

    // List of well known fields for Notification
    static const char * ID = "id";
    static const char * SUMMARY = "summary";
    static const char * BODY = "body";
    static const char * ICON = "icon";
    static const char * CLASS = "class";
    static const char * TIME_OUT = "time_out";
    static const char * URGENCY_LEVEL = "urgency_level";
    static const char * APPLICATION_NAME = "application_name";
    static const char * ACTIONS = "actions";

    typedef QVariantMap Notification;

    class Stack : public QObject {

        Q_OBJECT
    public:

        virtual ~Stack() {
        }

        /**
         * Adds a notification to the top of the stack.
         * The signal <code>notificationAdded</code> is throw.
         * @param notification
         * @return Notification id.
         */
        virtual int add(Notification notification) = 0;
        /**
         * Removes a notification from the stack.
         * The signal <code>notificationRemoved</code> is throw.
         * @param notificationID
         */
        virtual void removeByID(uint notificationID) = 0;

        /**
         * Remove the item at position "pos".
         * @param pos 
         */
        virtual void removeByPosition(int pos) = 0;

        /**
         * Udates the attributes of a given notification.
         * The signal <code>notificationUpdated</code> is throw.
         * @param notificationID
         * @param notification
         */
        virtual void update(uint notificationID, Notification notification) = 0;

        virtual Notification top() = 0;
        virtual const Notification at(int pos) = 0;
        virtual const Notification find(uint notificationID) = 0;
        virtual int size() = 0;

    signals:
        void notificationAdded(uint notificationId);
        void notificationRemoved(uint notificationId);
        void notificationChanged(uint notificationId);
    };
}
Q_DECLARE_INTERFACE(model_notifications::Stack, "org.moonlightde.model.notification.NotificationsStack/1.0")
US_DECLARE_SERVICE_INTERFACE(model_notifications::Stack, "org.moonlightde.model.notification.NotificationsStack/1.0")
#endif	/* NOTIFICATIONSSTACK_H */

