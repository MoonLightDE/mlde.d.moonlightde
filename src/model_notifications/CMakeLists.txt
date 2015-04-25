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


project(model_notifications)
## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
file(GLOB ${PROJECT_NAME}_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/*.h)

#file(GLOB ${PROJECT_NAME}_SOURCES c++/*.cpp)
set(${PROJECT_NAME}_SOURCES 
    c++/Activator.cpp
    c++/StackImpl.cpp
    c++/NotificationsDbusInterface.cpp
)


## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

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

## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

qt5_add_dbus_adaptor(${PROJECT_NAME}_SOURCES
    res/org.freedesktop.Notifications.xml
    c++/NotificationsDbusInterface.h NotificationsDbusInterface
)

set(GLOBAL_SRCS
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_PUBLIC_HEADERS}
)

m_build_module(
    NAME ${PROJECT_NAME}
    SOURCES ${GLOBAL_SRCS}
)


qt5_use_modules(${TARGET_NAME} Core Widgets DBus)
target_link_libraries( ${TARGET_NAME} 
    ${QTXDG_LIBRARIES}
    ${LXQT_LIBRARIES}
    
    Qt5::DBus
    Qt5::Widgets
)

m_deploy_module(${TARGET_NAME})