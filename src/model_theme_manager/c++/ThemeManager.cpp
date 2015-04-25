/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * 
 * Authors:
 *  Alexis López Zubieta
 * 
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

#include "ThemeManager.h"

#include <QIcon>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QApplication>

ThemeManager::ThemeManager() {
    m_paths << "/usr/share/themes";
    m_paths << QDir::homePath() + "/.themes";
}

ThemeManager::~ThemeManager() {
}

void ThemeManager::loadModuleQSS(const QString &moduleName) {
    if (m_qssThemeName.isNull()) {
        qDebug() << "No qss theme defined.";
        return;
    }

    foreach(QString path, m_paths) {
        QFile file(path + "/" + m_qssThemeName + "/" + qApp->applicationName() + "/" + moduleName + ".qss");
        qDebug() << file.fileName();
        if (file.exists()) {

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return;

            QTextStream in(&file);
            QString theme = in.readAll();
            m_qss.insert(moduleName, theme);
            refreshStyle();
        }
    }
}

void ThemeManager::removeQssTheme(QString qssThemeName) {
    m_qss.remove(qssThemeName);
    refreshStyle();
}

void ThemeManager::setQssTheme(QString qssThemeName) {
    m_qssThemeName = qssThemeName;
    qApp->setStyleSheet(qssThemeName);
}

void ThemeManager::setIconTheme(QString iconThemeName) {
    m_iconThemeName = iconThemeName;
    QIcon::setThemeName(iconThemeName);
}

void ThemeManager::refreshStyle() {
    QString global;

    foreach(QString theme, m_qss.values()) {
        global += theme;
    }
    qApp->setStyleSheet(global);
}