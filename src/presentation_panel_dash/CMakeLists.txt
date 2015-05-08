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


project(presentation_panel_dash)

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
file(GLOB ${PROJECT_NAME}_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/include/core/*.h)

file(GLOB ${PROJECT_NAME}_SOURCES c++/*.cpp)

file(GLOB ${PROJECT_NAME}_UIS ui/*.ui)

file(GLOB ${PROJECT_NAME}_RESOURCES res/*)
list(REMOVE_ITEM ${PROJECT_NAME}_RESOURCES "${PROJECT_SOURCE_DIR}/res/descriptor.desktop.in")


## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

## Qt
find_package(Qt5Core        REQUIRED)
find_package(Qt5Gui         REQUIRED)
find_package(Qt5Widgets     REQUIRED)
find_package(Qt5DBus        REQUIRED)
find_package(Qt5X11Extras   REQUIRED)

find_package(PkgConfig)
pkg_check_modules(SYSTEM_LIBS REQUIRED
    glib-2.0
    gio-2.0
    gio-unix-2.0
)

## CppMicroServices
find_package(CppMicroServices NO_MODULE REQUIRED)

include_directories(
    ${SYSTEM_LIBS_INCLUDE_DIRS}
    ${CppMicroServices_INCLUDE_DIRS}
)

link_directories(
    ${SYSTEM_LIBS_LIBRARY_DIRS}
)

## LXQT -- depends on QT5XDG
find_package(LXQT REQUIRED )
include(${LXQT_USE_FILE})

## Menu-Cache
find_package(PkgConfig)
pkg_check_modules(MENU_CACHE REQUIRED
    libmenu-cache>=0.3.3
)
include_directories(${MENU_CACHE_INCLUDE_DIRS})
add_definitions(-DHAVE_MENU_CACHE=1)

find_package(KF5WindowSystem REQUIRED)

## Inter-module dependencies
set(${PROJECT_NAME}_DEPENDS )

## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_BINARY_DIR})
QT5_WRAP_UI(${PROJEC_NAME}_UI_HEADERS ${${PROJECT_NAME}_UIS})
QT5_ADD_RESOURCES(${PROJEC_NAME}_QRC_SOURCES ${${PROJEC_NAME}_RESOURCES})

## --------------------------------------------------------------
## Build
## --------------------------------------------------------------
set (${PROJECT_NAME}_SOURCES
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJEC_NAME}_UI_HEADERS}
    ${${PROJECT_NAME}_RESOURCES}
)


m_build_module(
    NAME ${PROJECT_NAME}
    SOURCES ${${PROJECT_NAME}_SOURCES}
)
message(STATUS "SYSTEM_LIB ${SYSTEM_LIBS_LIBRARIES}")
target_link_libraries(${TARGET_NAME}
    ${LXQT_LIBRARIES}

    KF5::WindowSystem
     
    ${SYSTEM_LIBS_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::DBus
    Qt5::X11Extras
)

m_deploy_module(${TARGET_NAME})