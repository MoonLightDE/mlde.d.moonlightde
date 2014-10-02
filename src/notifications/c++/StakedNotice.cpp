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

#include "StakedNotice.h"
#include "NotificationMemo.h"

#include <QDebug>
#include <QMouseEvent>
#include <LXQt/XfitMan>

StakedNotice::StakedNotice(NotificationMemo *memo, QWidget * parent) : QWidget(parent) {
    widget.setupUi(this);

    widget.labelTitle->setText(memo->getSummary());
    widget.labelMsg->setText(memo->getBody());
    widget.labelIcon->setPixmap(memo->getPixmap());
    
    // Set a longer timeout for panel notifications

    m_timer.start(memo->getTimeout() * 2);

    connect(widget.pushButton, &QPushButton::released, this, &StakedNotice::dismiss);
    
    connect(&m_timer, &QTimer::timeout, memo, &NotificationMemo::expired);
    connect(memo, &NotificationMemo::expired, this, &StakedNotice::noticeDroped);
    connect(memo, &NotificationMemo::droped, this, &StakedNotice::noticeDroped);

    m_notice = memo;
}

StakedNotice::~StakedNotice() {
}

void StakedNotice::mouseReleaseEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
        // TODO: Implement actions support
        QString appName, windowTitle;

        foreach(Window i, LxQt::xfitMan().getClientList()) {
            appName = LxQt::xfitMan().getApplicationName(i);
            windowTitle = LxQt::xfitMan().getWindowTitle(i);

            if (!m_notice.isNull() &&
                    (appName == m_notice->getApplication() || windowTitle == m_notice->getSummary())) {

                LxQt::xfitMan().raiseWindow(i);
                hide();

                return;
            }
        }
    }

}