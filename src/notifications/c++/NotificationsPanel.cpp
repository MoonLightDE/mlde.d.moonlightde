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

#include "NotificationsPanel.h"


#include <LXQt/XfitMan>

#include <QRect>
#include <QList>
#include <QPointer>
#include <QRect>
#include <QPoint>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <qt5/QtCore/qsize.h>

NotificationsPanel::NotificationsPanel() {
    widget.setupUi(this);

    setWindowFlags(Qt::X11BypassWindowManagerHint
            | Qt::FramelessWindowHint
            | Qt::WindowStaysOnTopHint
            | Qt::Popup);

    connect(widget.pushButton, &QPushButton::released, this, &NotificationsPanel::dismissAll);
}

void NotificationsPanel::show() {
    QRect screenGeometry = LxQt::xfitMan().availableGeometry(this);
    setMinimumWidth(screenGeometry.width() * 0.3);
    setMaximumWidth(screenGeometry.width() * 0.3);

    setMinimumHeight(screenGeometry.height());

    QPoint pos = screenGeometry.topRight();
    pos.rx() -= width();
    move(pos);

    updateLayout();
    QWidget::show();
}

NotificationsPanel::~NotificationsPanel() {
}

void NotificationsPanel::stackNotification(NotificationMemo * memo) {
    QPointer<StakedNotice> notice = new StakedNotice(memo, this);
    connect(notice.data(), &StakedNotice::dismiss, this, &NotificationsPanel::dismissNotice);
    connect(notice.data(), &StakedNotice::noticeDroped, this, &NotificationsPanel::dropNotice);

    notices.append(notice);

    if (isVisible())
        updateLayout();
}

void NotificationsPanel::cleanLayout() {
    if (widget.scrollAreaWidgetNotices->layout() != NULL) {
        QLayoutItem* item;
        while ((item = widget.scrollAreaWidgetNotices->layout()->takeAt(0)) != NULL) {
            delete item;
        }
        delete widget.scrollAreaWidgetNotices->layout();
    }
}

void NotificationsPanel::updateLayout() {
    cleanLayout();

    QVBoxLayout * layout = new QVBoxLayout(widget.scrollAreaWidgetNotices);
    layout->setMargin(0);
    layout->setSpacing(0);

    foreach(QPointer<StakedNotice> notice, notices) {
        if (notice->getNotice().isNull())
            emit notice->noticeDroped();
        else {
            layout->addWidget(notice);
            notice->resize(200, 100);
        }

    }
    // Push up the notices and keep them tight
    layout->insertStretch(-1, 1);

    widget.scrollAreaWidgetNotices->setLayout(layout);
}

void NotificationsPanel::dismissAll() {

    foreach(QPointer<StakedNotice> notice, notices) {
        if (widget.scrollAreaWidgetNotices->layout() != NULL)
            widget.scrollAreaWidgetNotices->layout()->removeWidget(notice.data());
        if (!notice.isNull() && !notice->getNotice().isNull()) {
            emit notice->getNotice()->dismissed();
            notice.data()->deleteLater();
        }
    }
    notices.clear();

    updateLayout();
}

void NotificationsPanel::dropNotice() {
    StakedNotice * notice = qobject_cast<StakedNotice *>(QObject::sender());
    if (notice) {
        if (widget.scrollAreaWidgetNotices->layout() != NULL)
            widget.scrollAreaWidgetNotices->layout()->removeWidget(notice);
        notices.removeAll(notice);

        notice->deleteLater();
        updateLayout();
    }
}

void NotificationsPanel::dismissNotice() {
    StakedNotice * notice = qobject_cast<StakedNotice *>(QObject::sender());
    if (notice != NULL) {
        if (widget.scrollAreaWidgetNotices->layout() != NULL)
            widget.scrollAreaWidgetNotices->layout()->removeWidget(notice);

        notices.removeAll(notice);

        if (!notice->getNotice().isNull()) {
            emit notice->getNotice()->dismissed();
        }

        delete notice;
        updateLayout();
    }
}