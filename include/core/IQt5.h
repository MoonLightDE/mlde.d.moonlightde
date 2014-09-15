/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
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

#ifndef IQT5_H
#define IQT5_H

#include <usServiceInterface.h>
#include <QObject>
#include <QWidget>
#include <QSettings>

Q_DECLARE_INTERFACE(QObject, "org.qt-project.Qt.QObject")
US_DECLARE_SERVICE_INTERFACE(QObject, "org.qt-project.Qt.QObject")

US_DECLARE_SERVICE_INTERFACE(QWidget, "org.qt-project.Qt.QWidget")

US_DECLARE_SERVICE_INTERFACE(QSettings, "org.qt-project.Qt.QSettings")
#endif // IQT5_H
