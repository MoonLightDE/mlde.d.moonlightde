#    Copyright (C) 2015 Moonlight Desktop Environment Team
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

project(model_filesystem_gvfs)

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
file(GLOB ${PROJECT_NAME}_SOURCES c++/*.cpp)

file(GLOB ${PROJECT_NAME}_UIS ui/*.ui)

file(GLOB ${PROJECT_NAME}_RESOURCES res/*)
list(REMOVE_ITEM ${PROJECT_NAME}_RESOURCES "${PROJECT_SOURCE_DIR}/res/descriptor.desktop.in")


## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

## Qt
find_package(Qt5Core  REQUIRED)
find_package(Qt5Widgets  REQUIRED)
find_package(Qt5Gui  REQUIRED)

find_package(PkgConfig)
pkg_check_modules(SYSTEM_LIBS REQUIRED
    glib-2.0
    gio-2.0
    gio-unix-2.0
)

pkg_check_modules(LIBFM REQUIRED libfm>=1.2.0)

include_directories(
    ${SYSTEM_LIBS_INCLUDE_DIRS}
    ${LIBFM_INCLUDE_DIRS}
    )

link_directories(
    ${LIBFM_LIBRARY_DIRS}
    ${SYSTEM_LIBS_LIBRARY_DIRS}
)

## Inter-module dependencies
set(${PROJECT_NAME}_DEPENDS )

## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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
    DEPENDENCIES core
)

target_link_libraries(${TARGET_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui

    ${SYSTEM_LIBS_LIBRARIES}
    ${LIBFM_LIBRARIES}
 )

m_deploy_module(${TARGET_NAME})