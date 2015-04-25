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

#ifndef STACKIMPL_H
#define	STACKIMPL_H

#include "model_notifications/Stack.h"

#include <QMap>
#include <QStack>

using namespace model_notifications;

class StackImpl : public model_notifications::Stack {
    Q_INTERFACES(model_notifications::Stack)
    Q_OBJECT
public:
    StackImpl();
    virtual ~StackImpl();


    virtual int add(Notification notification);
    virtual void removeByID(uint notificationID);
    virtual void removeByPosition(int pos);
    virtual void update(uint notificationID, Notification notification);


    virtual Notification top();
    virtual const Notification at(int pos);
    virtual const Notification find(uint notificationID);


    virtual int size();

//signals:
//    void notificationAdded(uint notificationId);
//    void notificationRemoved(uint notificationId);
//    void notificationChanged(uint notificationId);
    
private:
    QStack<model_notifications::Notification> m_Stack;
    uint m_NextId;
};

#endif	/* STACKIMPL_H */

