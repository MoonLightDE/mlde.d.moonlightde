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

#ifndef _STACKEDNOTICE_H
#define	_STACKEDNOTICE_H

#include "ui_stackedNotice.h"

#include "NotificationMemo.h"
#include <QPointer>
#include <QTimer>

class StakedNotice : public QWidget {
    Q_OBJECT
public:
    StakedNotice(NotificationMemo * memo, QWidget * parent = 0);
    virtual ~StakedNotice();

    QPointer<NotificationMemo> getNotice() const {
        return m_notice;
    }

signals:
    void dismiss();
    void noticeDroped();

protected:
    void mouseReleaseEvent(QMouseEvent * event);

private:
    Ui::stackedNotice widget;
    QPointer<NotificationMemo> m_notice;
    QTimer m_timer;
};

#endif	/* _STACKEDNOTICE_H */
