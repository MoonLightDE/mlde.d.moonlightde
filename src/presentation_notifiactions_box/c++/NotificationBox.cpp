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

#include "NotificationBox.h"
#include "NotificationsController.h"

#include <QDebug>
#include <QPoint>
#include <QPixmap>
#include <QVariant>
#include <QDesktopWidget>
#include <QAbstractButton>
#include <QMutexLocker>

#include <KF5/KWindowSystem/KWindowSystem>

NotificationBox::NotificationBox(QWidget * parent) : QWidget(parent), ICONSIZE(48, 48) {
    widget.setupUi(this);
    setObjectName("NotificationBox");
    setWindowFlags(Qt::X11BypassWindowManagerHint
            | Qt::FramelessWindowHint
            | Qt::WindowStaysOnTopHint);

    connect(widget.pushButton, &QAbstractButton::released, this, &NotificationBox::showAllNotifications);
    connect(widget.pushButton, &QAbstractButton::released, this, &QWidget::hide);
    
    
}

NotificationBox::~NotificationBox() {
}

void NotificationBox::show(int timeOut) {
    // Reset mouse tracking
    m_hasMouseOver = false;

    // Reset postition
    int screenNumber = QApplication::desktop()->screenNumber(this);
    QPoint pos = KWindowSystem::workArea(screenNumber).topRight();

    pos.rx() -= width() - 1;
    pos.ry() += 20;
    move(pos);

    m_timer.start(timeOut);

    connect(&m_timer, &QTimer::timeout, this, &NotificationBox::hide);

    updateGeometry();
    QWidget::show();
}

void NotificationBox::hide() {
    disconnect(&m_timer, &QTimer::timeout, NULL, NULL);

    m_timer.stop();

    if (m_hasMouseOver)
        return;

    QWidget::hide();
}

void NotificationBox::enterEvent(QEvent * event) {
    m_hasMouseOver = true;
}

void NotificationBox::leaveEvent(QEvent * event) {
    m_hasMouseOver = false;

    hide();
}

void NotificationBox::mouseReleaseEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton)
        emit(defaultActionTriggered());
    else if(event->button() == Qt::RightButton)
        emit(userClosed());
}

void NotificationBox::setStackedNotices(int stackedNotices) {
    if (stackedNotices <= 1) {
        widget.aditionalWidget->setVisible(false);
        widget.number->setText(0);
    } else {
        widget.number->setText(QString::number(stackedNotices));
        widget.aditionalWidget->setVisible(true);
    }
}

void NotificationBox::setIcon(const QPixmap& pixmap) {
    if (pixmap.isNull()) {
        widget.iconWidget->setVisible(false);
        return;
    }

    if (pixmap.size().width() > ICONSIZE.width()
            || pixmap.size().height() > ICONSIZE.height())
        widget.labelIcon->setPixmap(pixmap.scaled(ICONSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        widget.labelIcon->setPixmap(pixmap);

    widget.iconWidget->setVisible(true);
}

void NotificationBox::setSummary(const QString& summary) {
    widget.labelTitle->setText(summary);
}

void NotificationBox::setBody(const QString& body) {
    widget.labelMsg->setText(body);
}

void NotificationBox::clear() {
    widget.labelIcon->clear();
    widget.iconWidget->hide();

    widget.labelTitle->clear();
    widget.labelMsg->clear();

    widget.aditionalWidget->hide();
}

void NotificationBox::wheelEvent(QWheelEvent* event) {

        int direction = event->delta();
        if (direction > 0) {
            emit(showPreviousNotice());
        } else {
            emit(showNextNotice());
        }
}
