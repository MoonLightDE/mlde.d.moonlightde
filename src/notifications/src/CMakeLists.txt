#    Copyright (C) 2014 Moonlight Desktop Environment Team
#    Authors:
#       Alexis López Zubieta
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

set(MODULE_NAME Notifications_Service)
set(MODULE_FULL_NAME "MoonLightDE Panel")
project(moonlightDE-${MODULE_NAME})

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QTX_INCLUDE_DIRS}

    ${INTERFACES_DIR}
)

set(NOTIFICATIONS_SRC
#    main.cpp
    Activator.cpp
    notificationarea.cpp
    notification.cpp
    notificationlayout.cpp
    notificationwidgets.cpp
    notifyd.cpp
    ${INTERFACES_DIR}/notifications/INotificationsService.h
)

set(NOTIFICATIONS_UI
    notification.ui
)

## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

if(USE_QT5)
    set(QTX_INCLUDE_DIRS "")
    set(QTX_LIBRARIES Qt5::Widgets Qt5::DBus)
else()
    set(QTX_INCLUDE_DIRS ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTDBUS_DIR})
    set(QTX_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTDBUS_LIBRARY})
endif()

## X11
find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})


## CppMicroServices
find_package(CppMicroServices NO_MODULE REQUIRED)
include_directories(${CppMicroServices_INCLUDE_DIRS})


## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------

usFunctionGenerateModuleInit( NOTIFICATIONS_SRC
        NAME ${MODULE_FULL_NAME}
        LIBRARY_NAME "${MODULE_NAME}"
)

if(USE_QT5)
    qt5_wrap_ui(NOTIFICATIONS_UI_CPP ${NOTIFICATIONS_UI})
    qt5_add_dbus_adaptor(NOTIFICATIONS_SRC
        org.freedesktop.Notifications.xml
        notifyd.h Notifyd
    )
else()
    qt4_wrap_ui(NOTIFICATIONS_UI_CPP ${NOTIFICATIONS_UI})
    qt4_add_dbus_adaptor(NOTIFICATIONS_SRC
        org.freedesktop.Notifications.xml
        notifyd.h Notifyd
    )
endif()

# Translations **********************************
include(LxQtTranslate)
lxqt_translate_ts(NOTIFICATIONS_QM_FILES
    SOURCES
        ${NOTIFICATIONS_SRC}
        ${NOTIFICATIONS_UI}
)
#************************************************

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${MoonLightDEPrefix}/modules)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED 
    ${NOTIFICATIONS_SRC}
    ${NOTIFICATIONS_UI_CPP}
    ${NOTIFICATIONS_QM_FILES}
)

#add_executable(lxqt-notificationd
#    ${NOTIFICATIONS_SRC}
#    ${NOTIFICATIONS_UI_CPP}
#    ${NOTIFICATIONS_QM_FILES}
#)

target_link_libraries(${PROJECT_NAME}
    ${QTX_LIBRARIES}
    ${QTXDG_LIBRARIES}
    ${LXQT_LIBRARIES}
    ${CppMicroServices_LIBRARIES}
)

## --------------------------------------------------------------
## Deploy
## --------------------------------------------------------------
INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${MODULES_OUTPUT_DIR}
    ARCHIVE DESTINATION lib/static)

INSTALL(FILES ${${PROJECT_NAME}_PUBLIC_HEADERS} DESTINATION include/${MoonLightDEPrefix} )
