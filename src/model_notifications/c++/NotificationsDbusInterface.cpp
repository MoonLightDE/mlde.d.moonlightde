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

#include "NotificationsDbusInterface.h"

#include <qt5xdg/XdgIcon>

#include <QUrl>
#include <QFile>
#include <QIcon>
#include <QByteArray>
#include <QApplication>
#include <QDBusArgument>
#include <QDebug>

NotificationsDbusInterface::NotificationsDbusInterface(model_notifications::Stack * stack) : QObject(stack), m_Stack(stack) {
}

NotificationsDbusInterface::~NotificationsDbusInterface() {
}

void NotificationsDbusInterface::CloseNotification(uint id) {
    m_Stack->removeByID(id);
    emit NotificationClosed(id, 3);
}

QStringList NotificationsDbusInterface::GetCapabilities() {
    QStringList caps;
    caps
            // << "actions"
            // << "action-icons"
            << "body"
            << "body-hyperlinks"
            << "body-images"
            << "body-markup"
            // << "icon-multi"
            // << "icon-static"
            << "persistence"
            // << "sound"
            ;
    return caps;
}

QString NotificationsDbusInterface::GetServerInformation(QString& vendor, QString& version, QString& spec_version) {
    spec_version = QString("1.2");
    version = QString(qApp->applicationVersion());
    vendor = QString(qApp->organizationDomain());
    return QString(qApp->applicationName());
}

uint NotificationsDbusInterface::Notify(const QString& app_name,
        uint replaces_id,
        const QString& app_icon,
        const QString& summary,
        const QString& body,
        const QStringList& actions,
        const QVariantMap& hints,
        int expire_timeout) {

    // Fill notification struct
    model_notifications::Notification notification;
    notification.insert(model_notifications::APPLICATION_NAME, app_name);
    notification.insert(model_notifications::SUMMARY, summary);
    notification.insert(model_notifications::BODY, body);
    notification.insert(model_notifications::ACTIONS, actions);

    // handling the "server decides" timeout
    if (expire_timeout == -1) {
        // TODO: Implement using qsettings
        expire_timeout = 5;
        expire_timeout *= 1000;
    }
    notification.insert(model_notifications::TIME_OUT, expire_timeout);

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
    } else if (!app_icon.isEmpty()) {
        pixmap = getPixmapFromString(app_icon);
        //        qDebug() << application << "from icon" << icon << m_pixmap.isNull();
    } else if (!hints["icon_data"].isNull()) {
        pixmap = getPixmapFromHint(hints["icon_data"]);
        //       qDebug() << application << "from icon_data" << m_pixmap.isNull();
    }

    notification.insert(model_notifications::ICON, pixmap);
    uint id;
    if (replaces_id == 0) {
        id = m_Stack->add(notification);
    } else {
        id = replaces_id;
        m_Stack->update(replaces_id, notification);
    }
    return id;
}

QPixmap NotificationsDbusInterface::getPixmapFromHint(const QVariant &argument) const {
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

QPixmap NotificationsDbusInterface::getPixmapFromString(const QString &str) const {
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