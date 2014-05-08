/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 * Alexis López Zubieta
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


#include "SettingsProfile.h"

#include <QApplication>
#include <QStringList>
#include <QUuid>


SettingsProfile::SettingsProfile(QString profileName):profileName(profileName) {

}
QSettings * SettingsProfile::getSettingsOf(QObject * object) {
    QStringList path;
    QObject * itr = object;
    while (itr != NULL) {
        if (! itr->objectName().isEmpty())
            path.push_front(itr->objectName());
        itr = itr->parent();
    }

    path.push_front(profileName);
    return new QSettings(qApp->applicationName(), path.join("/"));
}

QSettings * SettingsProfile::getSettingsOf(const QString & objectPath) {
    QStringList path;
    path.push_front(profileName);
    path.push_back(objectPath);
    return new QSettings(qApp->applicationName(), path.join("/"));
}


SettingsProfile::~SettingsProfile() {
}

