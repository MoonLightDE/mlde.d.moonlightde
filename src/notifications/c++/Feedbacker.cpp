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

#include <QDebug>

#include "Feedbacker.h"
#include "NotificationMemo.h"


#include <qt5xdg/XdgIcon>

Feedbacker::Feedbacker() : m_messageBox(NULL), NOTIFICATION_TIMEOUT_EXTENDED(60000) {
    qDebug() << "Preparing dbus service.";
    m_dbusManager = new NotificationsDbusInterface(this);
    m_dbus_adaptor = new NotificationsAdaptor(m_dbusManager);

    connect(m_dbusManager, SIGNAL(notificationAdded(uint, QString, QString, QString, QString, int, QStringList, QVariantMap)),
            this, SLOT(notify(uint, QString, QString, QString, QString, int, QStringList, QVariantMap)));

    connect(m_dbusManager, SIGNAL(notificationClosed(uint, uint)),
            m_dbus_adaptor, SIGNAL(NotificationClosed(uint, uint)));

    connect(&m_messageBox, &NotificationBox::showAllNotifications, this, &Feedbacker::showNotificationsPanel);

    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerService("org.freedesktop.Notifications"))
        qDebug() << "registerService failed: another service with 'org.freedesktop.Notifications' runs already";
    if (!connection.registerObject("/org/freedesktop/Notifications", m_dbusManager))
        qDebug() << "registerObject failed: another object with '/org/freedesktop/Notifications' runs already";

    m_panel.show();
}

Feedbacker::~Feedbacker() {
    delete m_dbus_adaptor;
    delete m_dbusManager;
}

void Feedbacker::notify(uint id, const QString &application,
        const QString &summary, const QString &body,
        const QString &icon, int timeout,
        const QStringList& actions, const QVariantMap& hints) {

    // If the notifications exist remove it
    if (m_notifications.contains(id)) {
        emit m_notifications.value(id)->droped();
        delete m_notifications.value(id);
        m_notifications.remove(id);
    }
    NotificationMemo *memo = new NotificationMemo(application, summary, body, icon, timeout, actions, hints);

    // Get notification pixmap
    // Notifications spec set real order here:
    // An implementation which only displays one image or icon must
    // choose which one to display using the following order:
    //  - "image-data"
    //  - "image-path"
    //  - app_icon parameter
    //  - for compatibility reason, "icon_data"
    QPixmap pixmap;
    if (!hints["image_data"].isNull()) {
        pixmap = getPixmapFromHint(hints["image_data"]);
        //        qDebug() << application << "from image_data" << m_pixmap.isNull();
    } else if (!hints["image_path"].isNull()) {
        pixmap = getPixmapFromString(hints["image_path"].toString());
        //        qDebug() << application << "from image_path" << m_pixmap.isNull();
    } else if (!icon.isEmpty()) {
        pixmap = getPixmapFromString(icon);
        //        qDebug() << application << "from icon" << icon << m_pixmap.isNull();
    } else if (!hints["icon_data"].isNull()) {
        pixmap = getPixmapFromHint(hints["icon_data"]);
        //       qDebug() << application << "from icon_data" << m_pixmap.isNull();
    }
    memo->setPixmap(pixmap);


    // Prevent the duplication of the same notification
    foreach(NotificationMemo *itr, m_notifications.values()) {
        if (itr == memo)
            return;
    }


    if (!m_panel.isVisible()) {
        // Show message box
        
        m_messageBox.setCurrentNotice(memo);
        m_messageBox.setHidenNotices(m_notifications.size());
        
        m_notifications.insert(id, memo);
        m_messageBox.show();
    }
    
    connect(memo, &NotificationMemo::expired, this, &Feedbacker::handleNotificationTimeOut);
    connect(memo, &NotificationMemo::dismissed, this, &Feedbacker::handleNotificationDismiss);


    // Stack notice
    m_panel.stackNotification(memo);

//    TODO: Implement actions support
//    connect(memo, SIGNAL(actionTriggered(QString)),
//            this, SLOT(handleActionInvokation(QString)));*
  
}

void Feedbacker::showNotificationsPanel() {
    m_panel.show();
}

uint Feedbacker::registerTask(const QWidget *taskCtrl) {
    return 0;
}

QPixmap Feedbacker::getPixmapFromHint(const QVariant &argument) const {
    int width, height, rowstride, bitsPerSample, channels;
    bool hasAlpha;
    QByteArray data;

    const QDBusArgument arg = argument.value<QDBusArgument>();
    arg.beginStructure();
    arg >> width;
    arg >> height;
    arg >> rowstride;
    arg >> hasAlpha;
    arg >> bitsPerSample;
    arg >> channels;
    arg >> data;
    arg.endStructure();
    QImage img = QImage((uchar*) data.constData(), width, height, QImage::Format_ARGB32).rgbSwapped();

    return QPixmap::fromImage(img);
}

QPixmap Feedbacker::getPixmapFromString(const QString &str) const {
    QUrl url(str);
    if (url.isValid() && QFile::exists(url.toLocalFile())) {
        //        qDebug() << "    getPixmapFromString by URL" << url;
        return QPixmap(url.toLocalFile());
    } else {
        //        qDebug() << "    getPixmapFromString by XdgIcon theme" << str << ICONSIZE << XdgIcon::themeName();
        //        qDebug() << "       " << XdgIcon::fromTheme(str) << "isnull:" << XdgIcon::fromTheme(str).isNull();
        // They say: do not display an icon if it;s not found - see #325
        // TODO: Implement icon size setting using qsetting
        return XdgIcon::fromTheme(str/*, XdgIcon::defaultApplicationIcon()*/).pixmap(QSize(48, 48));
    }
}

/*
void Feedbacker::removeNotification(uint id, uint reason) {
    NotificationMemo *n = m_notifications.value(id);
    if (!n) {
        qWarning() << "Remove notification request fail.\n"
                "The notification with ID: " << id << " doesn't exist.";
        return;
    }

    delete n;
    m_notifications.remove(id);

    emit notificationClosed(id, reason);
}*/
void Feedbacker::CloseNotification(uint id) {
    NotificationMemo * memo = m_notifications.value(id);
    if (memo) {
        m_notifications.remove(id);
        memo->deleteLater();

        emit m_dbusManager->notificationClosed(id, 3);
    }
}

void Feedbacker::handleNotificationTimeOut() {
    NotificationMemo * memo = qobject_cast<NotificationMemo *>(QObject::sender());
    if (memo) {
        uint id = m_notifications.key(memo);
        m_notifications.remove(id);
        memo->deleteLater();

        emit m_dbusManager->notificationClosed(id, 1);
    } else {
        qDebug() << "handleNotificationTimeOut recieved from a non NotificationMeno.";
    }
}

void Feedbacker::handleNotificationDismiss() {
    NotificationMemo * memo = qobject_cast<NotificationMemo *>(QObject::sender());
    if (memo) {
        uint id = m_notifications.key(memo);
        m_notifications.remove(id);
        memo->deleteLater();
        emit m_dbusManager->notificationClosed(id, 2);
    } else {
        qDebug() << "handleNotificationDismiss recieved from a non NotificationMeno.";
    }
}

void Feedbacker::handleActionInvokation(const QString &action) {
    qDebug() << "Feedbacker::handleActionInvokation" << " not implemented yet.";
    /*NotificationMemo * memo = qobject_cast<NotificationMemo *>(QObject::sender());
    if (memo) {
        uint id = m_notifications.key(memo);
        emit m_dbusManager->ActionInvoked(id, action);
    }*/
}