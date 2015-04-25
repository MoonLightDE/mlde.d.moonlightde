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
#include "NotificationBox.h"
#include "NotificationsController.h"

#include <qt5xdg/XdgIcon>
#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/KWindowSystem/KWindowInfo>

#include <QDebug>

NotificationsController::NotificationsController() : m_Box(new NotificationBox()), m_Stack(NULL), m_Current(), currentPos(0), m_ScrollLock(false) {
    connect(m_Box, SIGNAL(userClosed()), this, SLOT(onUserClosed()));
}

NotificationsController::~NotificationsController() {
    delete m_Box;
}

void NotificationsController::setStack(model_notifications::Stack* stack) {
    if (stack == NULL) {
        qWarning() << MODULE_NAME << ": Setting an NULL stack";
    } else {
        m_Stack = stack;

        connect(m_Stack.data(), SIGNAL(notificationAdded(uint)), this, SLOT(onNotificationAdded(uint)));
        connect(m_Stack.data(), SIGNAL(notificationRemoved(uint)), this, SLOT(onNotificationRemoved(uint)));
        connect(m_Stack.data(), SIGNAL(notificationChanged(uint)), this, SLOT(onNotificationChanged(uint)));
    }
}

void NotificationsController::unsetStack() {
    disconnect(m_Stack.data(), NULL, this, NULL);
    m_Stack.clear();
}

void NotificationsController::showNotificationBox() {
    //m_Box->hide();
    m_Box->clear();

    // Setup new data
    m_Box->setIcon(m_Current.value(model_notifications::ICON).value<QPixmap>());
    m_Box->setSummary(m_Current.value(model_notifications::SUMMARY).toString());
    m_Box->setBody(m_Current.value(model_notifications::BODY).toString());
    m_Box->setStackedNotices(m_Stack->size());

    // Connect slots
    connect(m_Box, SIGNAL(defaultActionTriggered()), this, SLOT(onDefaultActionTriggered()));

    connect(m_Box, SIGNAL(showNextNotice()), this, SLOT(showNextNotification()));
    connect(m_Box, SIGNAL(showPreviousNotice()), this, SLOT(showPreviousNotification()));

    // Display
    m_Box->show(m_Current.value(model_notifications::TIME_OUT, 3000).toInt());
}

void NotificationsController::hideNotificationBox() {
    disconnect(m_Box, NULL, this, NULL);
    m_Box->hide();
    m_Box->clear();
}

void NotificationsController::onNotificationAdded(uint notificationId) {
    if (m_Box.isNull()) {
        m_Box = new NotificationBox();
    }

    m_Current = m_Stack->top();
    showNotificationBox();
}

void NotificationsController::onNotificationChanged(uint notificationId) {
    const model_notifications::Notification &notification = m_Stack->find(notificationId);
    if (notification.isEmpty())
        qWarning() << MODULE_NAME << " : notification identified as " << notificationId << " no longer exists.";
    else {
        if (notificationId == m_Current.value(model_notifications::ID).value<uint>()) {
            m_Current = notification;
            showNotificationBox();
        }
    }
}

void NotificationsController::onNotificationRemoved(uint notificationId) {
    const model_notifications::Notification &notification = m_Stack->find(notificationId);
    if (notification.isEmpty())
        qWarning() << MODULE_NAME << " : notification identified as " << notificationId << " no longer exists.";
    else {
        if (notificationId == m_Current.value(model_notifications::ID).value<uint>()) {
            m_Current = model_notifications::Notification();

            m_Box->hide();
            m_Box->clear();
        }
    }
}

void NotificationsController::onDefaultActionTriggered() {
    QString appName, windowTitle;

    const QString noticeAppName = m_Current.value(model_notifications::APPLICATION_NAME).toString();

    foreach(const WId &id, KWindowSystem::windows()) {
        KWindowInfo info(id, NET::WMVisibleName | NET::WMName);
        appName = info.name();
        windowTitle = info.visibleName();


        if (appName == noticeAppName || windowTitle == noticeAppName) {
            KWindowSystem::raiseWindow(id);
            m_Box->hide();
            return;
        }
    }
}

void NotificationsController::showNextNotification() {
    if (!m_ScrollLock) {
        m_ScrollLock = true;
        m_scrollTimer.singleShot(500, this, SLOT(releaseScroll()));
        if (currentPos < (m_Stack.data()->size() - 1)) {
            currentPos++;
            qDebug() << "showNextNotification: " << currentPos;
            m_Current = m_Stack.data()->at(currentPos);
        } else {
            return;
        }
        showNotificationBox();
    }
}

void NotificationsController::showPreviousNotification() {
    if (!m_ScrollLock) {
        m_ScrollLock = true;
        m_scrollTimer.singleShot(10, this, SLOT(releaseScroll()));
        if (currentPos > 0) {
            currentPos--;
            qDebug() << "showPreviousNotification: " << currentPos;
            m_Current = m_Stack.data()->at(currentPos);
        } else {
            currentPos = 0;
            return;
        }
        showNotificationBox();
    }
}

void NotificationsController::releaseScroll() {
    qDebug() << "Released ";
    m_ScrollLock = false;
}

void NotificationsController::onUserClosed() {
    qDebug() << "Rock u like a hurricane" << endl;
    hideNotificationBox();
}
