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
#include "model_notifications/Stack.h"

#include <QSize>
#include <QTimer>
#include <QPointer>
#include <QMouseEvent>
#include <QMutex>

class NotificationBox : public QWidget {
    Q_OBJECT
public:
    NotificationBox(QWidget * parent = NULL);
    virtual ~NotificationBox();

    void setIcon(const QPixmap &pixmap);
    void setSummary(const QString &summary);
    void setBody(const QString &body);
    
    void clear();
    
    void setStackedNotices(int stackedNotices);

public slots:
    void show(int timeOut);
    void hide();

signals:
    void showAllNotifications();
    void timeOut();
    void userClosed();
    void defaultActionTriggered();
    //void actionTrigered(QString action);

    void showNextNotice();
    void showPreviousNotice();
    
protected:
    void leaveEvent(QEvent * event);
    void enterEvent(QEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent * event);

private:
    bool m_hasMouseOver;

    Ui::NotificationBox widget;
    const QSize ICONSIZE;

    QTimer m_timer;

};

#endif	/* _NOTIFICATIONBOX_H */
