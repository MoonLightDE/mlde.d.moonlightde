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

#ifndef _NOTIFICATIONBOX_H
#define	_NOTIFICATIONBOX_H

#include "ui_NotificationBox.h"
#include "NotificationMemo.h"

#include <QSize>
#include <QTimer>
#include <QPointer>
#include <QMouseEvent>

class NotificationBox : public QWidget {
    Q_OBJECT
public:
    NotificationBox(QWidget * parent = NULL);
    virtual ~NotificationBox();

    QPointer<NotificationMemo> getCurrentNotice() const {
        return m_currentNotice;
    }

    void setCurrentNotice(NotificationMemo* currentNotice);

    int getHidenNotices() const {
        return m_hidenNotices;
    }

    void setHidenNotices(int hidenNotices);

public slots:
    void show();
    void hide();

signals:
    void showAllNotifications();

protected:
    void leaveEvent(QEvent * event);
    void enterEvent(QEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    bool m_hasMouseOver;

    Ui::NotificationBox widget;
    QPointer<NotificationMemo> m_currentNotice;
    int m_hidenNotices;
    const QSize ICONSIZE;
    
    QTimer m_timer;
};

#endif	/* _NOTIFICATIONBOX_H */
