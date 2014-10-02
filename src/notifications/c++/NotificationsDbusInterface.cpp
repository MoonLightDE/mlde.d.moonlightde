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

#include <QDebug>

NotificationsDbusInterface::NotificationsDbusInterface(QObject* parent) : QObject(parent), m_idCounter(0) {
}

NotificationsDbusInterface::~NotificationsDbusInterface() {
}

void NotificationsDbusInterface::CloseNotification(uint id) {
    emit notificationClosed(id, 3);
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
    //TODO: Add proper MoonLight Versioning using constants
    version = QString("0.7");
    vendor = QString("moonlightde.org");
    return QString("moonlightDE");
}

uint NotificationsDbusInterface::Notify(const QString& app_name,
        uint replaces_id,
        const QString& app_icon,
        const QString& summary,
        const QString& body,
        const QStringList& actions,
        const QVariantMap& hints,
        int expire_timeout) {

    uint id;
    if (replaces_id == 0) {
        m_idCounter++;
        id = m_idCounter;
    } else
        id = replaces_id;

//    qDebug() << QString("Notify(\n"
//            "  app_name = %1\n"
//            "  replaces_id = %2\n"
//            "  app_icon = %3\n"
//            "  summary = %4\n"
//            "  body = %5\n"
//            ).arg(app_name, QString::number(replaces_id), app_icon, summary, body)
//            << "  actions =" << actions << endl
//            << "  hints =" << hints << endl
//            << QString("  expire_timeout = %1\n) => %2").arg(QString::number(expire_timeout), QString::number(id));

    // handling the "server decides" timeout
    if (expire_timeout == -1) {
        // TODO: Implement using qsettings
        expire_timeout = 5;
        expire_timeout *= 1000;
    }

    emit notificationAdded(id, app_name, summary, body, app_icon, expire_timeout, actions, hints);

    return id;
}