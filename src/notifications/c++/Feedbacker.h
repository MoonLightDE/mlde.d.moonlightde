/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
 * 
 * This file is part of Moonlight Desktop Environment and it's based on the 
 * lxqt notifications system.
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

#ifndef FEEDBACKER_H
#define	FEEDBACKER_H

#include "NotificationsDbusInterface.h"

#include "notificationsadaptor.h"
#include "NotificationMemo.h"

#include "NotificationBox.h"
#include "NotificationsPanel.h"

#include <notifications/IFeedbacker.h>

#include <QObject>
#include <QHash>

class Feedbacker : public IFeedbacker {
    Q_OBJECT
    Q_INTERFACES(IFeedbacker)
public:
    Feedbacker();
    virtual ~Feedbacker();

public slots:

    /*! Handle close request from external app. 
     */
    void CloseNotification(uint id);

    /*! Handle close request by timeout.
     */
    void handleNotificationTimeOut();
    /*! Handle close request by user.
     */
    void handleNotificationDismiss();

    /*! Add new notification
     * See \c IFeedbacker::notify() for params meanings.
     */
    void notify(uint id, const QString &application,
            const QString &summary, const QString &body,
            const QString &icon, int timeout,
            const QStringList& actions, const QVariantMap& hints);

    void showNotificationsPanel();

    uint registerTask(const QWidget *taskCtrl);

    /* Handle action invocation from a NotificationMeno  */
    void handleActionInvokation(const QString &action);
signals:
    /*! Inform the external application that user chose one of provided action via the \c NotificationsDbusInterface
     * \param in0 a notification ID (obtained from \c NotificationsDbusInterface)
     * \param in1 a selected action key from the (key - display value) pair
     */
    void actionInvoked(uint id, const QString &actionKey);

    void notificationAdded(NotificationMemo * memo);
private:
    // Utility function to get the corresponding icon pixmap
    QPixmap getPixmapFromString(const QString &str) const;

    // Utility function to get the corresponding icon pixmap
    QPixmap getPixmapFromHint(const QVariant &argument) const;

    QHash<uint, QPointer<NotificationMemo> > m_notifications;

    QPointer<NotificationsDbusInterface> m_dbusManager;
    QPointer<NotificationsAdaptor> m_dbus_adaptor;
    NotificationBox m_messageBox;
    NotificationsPanel m_panel;

    const int NOTIFICATION_TIMEOUT_EXTENDED;
};

#endif	/* FEEDBACKER_H */

