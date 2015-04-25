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

#ifndef NOTIFICATIONSCONTROLLER_H
#define	NOTIFICATIONSCONTROLLER_H

#include "model_notifications/Stack.h"
#include "NotificationBox.h"

#include <QPointer>
#include <QObject>

class NotificationsController : public QObject {
    Q_OBJECT
public:
    NotificationsController();
    virtual ~NotificationsController();


    void setStack(model_notifications::Stack * stack);
    void unsetStack();

    /**
     * Displays a notification box with the information contained into m_Current
     * cocnects the box signals with this controller.
     */
    void showNotificationBox();

    /**
     * Hides the notification box and cleans its contents, also remove all 
     * conections.
     */
    void hideNotificationBox();

public slots:
    void onNotificationAdded(uint notificationId);
    void onNotificationChanged(uint notificationId);
    void onNotificationRemoved(uint notificationId);
    void onDefaultActionTriggered();

    void showNextNotification();
    void showPreviousNotification();
    void releaseScroll();
    void onUserClosed();
private:
    QPointer<model_notifications::Stack> m_Stack;
    QPointer<NotificationBox> m_Box;
    model_notifications::Notification m_Current;

    int currentPos;
    QTimer m_scrollTimer;
    bool m_ScrollLock;
};

#endif	/* NOTIFICATIONSCONTROLLER_H */

