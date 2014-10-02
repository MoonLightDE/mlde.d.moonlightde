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

#include <qt5xdg/XdgIcon>

#include <QDebug>
#include <QPoint>
#include <QPixmap>
#include <QAbstractButton>

#include <LXQt/XfitMan>

NotificationBox::NotificationBox(QWidget * parent) : QWidget(parent), m_currentNotice(NULL), m_hidenNotices(0), ICONSIZE(32, 32) {
    widget.setupUi(this);

    setWindowFlags(Qt::X11BypassWindowManagerHint
            | Qt::FramelessWindowHint
            | Qt::WindowStaysOnTopHint);

    connect(widget.pushButton, &QAbstractButton::released, this, &NotificationBox::showAllNotifications);
    connect(widget.pushButton, &QAbstractButton::released, this, &QWidget::hide);
}

NotificationBox::~NotificationBox() {
}

void NotificationBox::show() {
    if (m_currentNotice == NULL) {
        qDebug() << "Atempting to show an empty notification box.";
        return;
    }

    // Reset mouse tracking
    m_hasMouseOver = false;

    // Reset postition
    QPoint pos = LxQt::xfitMan().availableGeometry(this).topRight();

    pos.rx() -= width() - 1;
    pos.ry() += 20;
    move(pos);


    m_timer.start(m_currentNotice->getTimeout());

    connect(&m_timer, &QTimer::timeout, this, &NotificationBox::hide);

    connect(&m_timer, &QTimer::timeout, m_currentNotice.data(), &NotificationMemo::expired);
    connect(m_currentNotice.data(), &NotificationMemo::destroyed, this, &NotificationBox::hide);

    updateGeometry();
    QWidget::show();
}

void NotificationBox::hide() {
    if (!m_currentNotice.isNull()) {
        disconnect(&m_timer, &QTimer::timeout, m_currentNotice.data(), &NotificationMemo::expired);
        disconnect(m_currentNotice.data(), &NotificationMemo::destroyed, this, &NotificationBox::hide);
    }
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

    if (!m_currentNotice.isNull())
        emit m_currentNotice->dismissed();

    hide();
}

void NotificationBox::mouseReleaseEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
        // TODO: Implement actions support
        QString appName, windowTitle;

        foreach(Window i, LxQt::xfitMan().getClientList()) {
            appName = LxQt::xfitMan().getApplicationName(i);
            windowTitle = LxQt::xfitMan().getWindowTitle(i);

            if (!m_currentNotice.isNull() &&
                    (appName == m_currentNotice->getApplication() || windowTitle == m_currentNotice->getSummary())) {

                LxQt::xfitMan().raiseWindow(i);
                hide();

                return;
            }
        }
    }

}

void NotificationBox::setHidenNotices(int hidenNotices) {
    m_hidenNotices = hidenNotices;

    if (hidenNotices == 0) {
        widget.aditionalWidget->setVisible(false);
        widget.number->setText(0);
    } else {

        widget.number->setText(QString::number(hidenNotices));
        widget.aditionalWidget->setVisible(true);
    }
}

void NotificationBox::setCurrentNotice(NotificationMemo * currentNotice) {
    if (currentNotice == NULL) {
        qDebug() << "Attempting to show an invalid notice";
        return;
    }
    // Disconnect previous notice
    if (!m_currentNotice.isNull()) {
        m_timer.stop();

        disconnect(&m_timer, &QTimer::timeout, m_currentNotice.data(), &NotificationMemo::expired);
        disconnect(m_currentNotice.data(), &NotificationMemo::destroyed, this, &NotificationBox::hide);
    }

    m_currentNotice = currentNotice;

    // Icon is disabled by default
    widget.iconWidget->setVisible(false);

    if (!m_currentNotice->getPixmap().isNull()) {

        if (m_currentNotice->getPixmap().size().width() > ICONSIZE.width()
                || m_currentNotice->getPixmap().size().height() > ICONSIZE.height())
            widget.labelIcon->setPixmap(m_currentNotice->getPixmap().scaled(ICONSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            widget.labelIcon->setPixmap(m_currentNotice->getPixmap());
        widget.iconWidget->setVisible(true);
    }

    widget.labelTitle->setText(m_currentNotice->getSummary());
    widget.labelMsg->setText(m_currentNotice->getBody());
}
