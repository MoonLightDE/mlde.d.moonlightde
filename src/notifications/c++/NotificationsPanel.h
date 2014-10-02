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

#ifndef _NOTIFICATIONSPANEL_H
#define	_NOTIFICATIONSPANEL_H

#include "ui_NotificationsPanel.h"

#include "StakedNotice.h"

#include <QList>
#include <QPointer>
#include <QSpacerItem>

class NotificationsPanel : public QWidget {
    Q_OBJECT
public:
    NotificationsPanel();
    virtual ~NotificationsPanel();

    /* Puts the given notification into the list and assing a new timeout
     * (x2 the original)
     */
    void stackNotification(NotificationMemo * memo);


public slots:
    void show();

    /* Hide an specifice StackedNotice, don't calls dismiss.
     */
    void dropNotice();
    /* 
     * Hide the Staked notice that calls the slot and calls dismiss.
     */
    void dismissNotice();
    
    /* Dismiss all notices
     */
    void dismissAll();

private:
    void updateLayout();
    void cleanLayout();
    
    Ui::NotificationsPanel widget;
    QList<QPointer<StakedNotice> > notices;

};

#endif	/* _NOTIFICATIONSPANEL_H */
