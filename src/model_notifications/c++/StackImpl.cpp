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

#include "StackImpl.h"
#include "module_config.h"

#include <limits>

#include <QDebug>

StackImpl::StackImpl() : m_NextId(0), m_Stack() {
}

StackImpl::~StackImpl() {
}

int StackImpl::add(Notification notification) {
    uint id = m_NextId;
    m_NextId = (m_NextId + 1) % std::numeric_limits<quint32>::max();
    // Assignn an id to the notification
    notification.insert(model_notifications::ID, id);

    m_Stack.append(notification);

    qDebug() << MODULE_NAME << ": notification added " << notification;
    emit(notificationAdded(id));
    return id;
}

void StackImpl::removeByID(uint notificationID) {
    int pos = 0;
    for (; pos < m_Stack.size(); pos++) {
        uint id = m_Stack.at(pos).value(model_notifications::ID).toInt();
        if (id == notificationID)
            break;
    }
    if (pos < m_Stack.size()) {
        qDebug() << MODULE_NAME << ": notification removed " << m_Stack.at(pos);
        m_Stack.remove(pos);
    }

    emit(notificationRemoved(notificationID));
}

void StackImpl::removeByPosition(int pos) {
    uint notificationID = m_Stack.at(pos).value(model_notifications::ID).value<uint>();

    qDebug() << MODULE_NAME << ": notification removed " << m_Stack.at(pos);
    m_Stack.remove(pos);
    emit(notificationRemoved(notificationID));
}

Notification StackImpl::top() {
    return m_Stack.top();
}

void StackImpl::update(uint notificationID, Notification notification) {
    int pos = 0;
    for (; pos < m_Stack.size(); pos++) {
        int id = m_Stack.at(pos).value(model_notifications::ID).toInt();
        if (id == notificationID)
            break;
    }
    if (pos >= m_Stack.size()) {
        qDebug() << MODULE_NAME << ": atemting to update an invalid notification.";
        m_Stack.append(notification);
    } else
        m_Stack[pos] == notification;

    emit (notificationChanged(notificationID));
    qDebug() << MODULE_NAME << ": notification updated " << notification;
}

const Notification StackImpl::at(int pos) {
    return m_Stack.at(pos);
}

const Notification StackImpl::find(uint notificationID) {
    for (int pos = 0; pos < m_Stack.size(); pos++) {
        int id = m_Stack.at(pos).value(model_notifications::ID).toInt();
        if (id == notificationID)
            return m_Stack.at(pos);
    }
    return Notification();
}

int StackImpl::size() {
    return m_Stack.size();
}

