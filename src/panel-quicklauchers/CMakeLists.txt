#    Copyright (C) 2014 Moonlight Desktop Environment Team
#    Authors:
#       Alexis López Zubieta
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
set(MODULE_NAME PanelWidget_QuickLaunchers)
set(MODULE_FULL_NAME "MoonLightDE Panel")
project(moonlightDE-${MODULE_NAME})

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
set(${PROJECT_NAME}_HEADERS
    c++/lxqtquicklaunch.h
    c++/quicklaunchaction.h
    c++/quicklaunchbutton.h
)
set(${PROJECT_NAME}_PUBLIC_HEADERS
)

set(${PROJECT_NAME}_SOURCES 
    c++/Activator.cpp
    c++/lxqtquicklaunch.cpp
    c++/quicklaunchaction.cpp
    c++/quicklaunchbutton.cpp
)

set ( ${PROJEC_NAME}_UIS
)

set ( ${PROJEC_NAME}_RESOURCES
)

include_directories(${INTERFACES_DIR})
include_directories(${CMAKE_BINARY_DIR})

## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

## Qt
find_package(Qt5Core        REQUIRED)
find_package(Qt5Gui         REQUIRED)
find_package(Qt5Widgets     REQUIRED)
find_package(Qt5DBus        REQUIRED)
find_package(Qt5X11Extras   REQUIRED)

## CppMicroServices
find_package(CppMicroServices NO_MODULE REQUIRED)
include_directories(${CppMicroServices_INCLUDE_DIRS})

## LXQT -- depends on QT5XDG
find_package(LXQT REQUIRED )
include(${LXQT_USE_FILE})

## QT5XDG
find_package(QT5XDG REQUIRED)
include(${QTXDG_USE_FILE})

include_directories(${MENU_CACHE_INCLUDE_DIRS})
add_definitions(-DHAVE_MENU_CACHE=1)


## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_WRAP_UI(${PROJEC_NAME}_UI_HEADERS ${${PROJEC_NAME}_UIS})
QT5_ADD_RESOURCES(${PROJEC_NAME}_QRC_SOURCES ${${PROJEC_NAME}_RESOURCES})

usFunctionGenerateModuleInit( ${PROJECT_NAME}_SOURCES
        NAME ${MODULE_FULL_NAME}
        LIBRARY_NAME "${MODULE_NAME}"
)

## --------------------------------------------------------------
## Build & Link
## --------------------------------------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${MoonLightDEPrefix}/modules)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_PUBLIC_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJEC_NAME}_UI_HEADERS}
    ${${PROJEC_NAME}_QRC_SOURCES}
)

target_link_libraries( ${PROJECT_NAME} 
    ${CppMicroServices_LIBRARIES}
    ${LXQT_LIBRARIES}
    ${QTXDG_LIBRARIES}

    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::DBus
    Qt5::X11Extras
)
## --------------------------------------------------------------
## Deploy
## --------------------------------------------------------------
INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${MODULES_OUTPUT_DIR}
    ARCHIVE DESTINATION lib/static)

INSTALL(FILES ${${PROJECT_NAME}_PUBLIC_HEADERS} DESTINATION include/${MoonLightDEPrefix} )
