#    Copyright (C) 2014 Moonlight Desktop Environment Team
#    Authors:
#       Alexis López Zubieta.
#
#    This file is part of the Moonlight Desktop Environment
#
#    Moonlight Desktop Environment is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Moonlight Desktop Environment is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Moonlight Desktop Environment.  If not, see <http://www.gnu.org/licenses/>.
#


project(notifications)
## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
file(GLOB ${PROJECT_NAME}_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/*.h)

file(GLOB ${PROJECT_NAME}_SOURCES c++/*.cpp)

file(GLOB ${PROJECT_NAME}_UIS ui/*.ui)

file(GLOB ${PROJECT_NAME}_RESOURCES res/*)


## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

## QT
find_package(Qt5Widgets REQUIRED QUIET)
find_package(Qt5DBus REQUIRED QUIET)
find_package(Qt5LinguistTools REQUIRED QUIET)

## CppMicroServices
find_package(CppMicroServices NO_MODULE REQUIRED)
include_directories(${CppMicroServices_INCLUDE_DIRS})


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

## QT5XDG
find_package(qt5xdg REQUIRED)
include(${QTXDG_USE_FILE})

## LXQT 
find_package(lxqt-qt5 REQUIRED QUIET)
include(${LXQT_USE_FILE})


## Inter-module dependencies
set(${PROJECT_NAME}_DEPENDS )

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)


qt5_wrap_ui(${PROJEC_NAME}_UI_HEADERS ${${PROJECT_NAME}_UIS})
qt5_add_dbus_adaptor(${PROJECT_NAME}_SOURCES
    res/org.freedesktop.Notifications.xml
    c++/NotificationsDbusInterface.h NotificationsDbusInterface
)

set(GLOBAL_SRCS
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJEC_NAME}_UI_HEADERS}
    ${NOTIFICATIONS_QM_FILES}
)

MoonLightDE_CreateModule(${PROJECT_NAME} ${GLOBAL_SRCS})

target_link_libraries(moonlightDE-${PROJECT_NAME} 
    ${QTXDG_LIBRARIES}
    ${LXQT_LIBRARIES}
    
    Qt5::DBus
    Qt5::Widgets
)
