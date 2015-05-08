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
project(presentation_panel_datetime)

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
file(GLOB ${PROJECT_NAME}_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/include/core/*.h)

file(GLOB ${PROJECT_NAME}_SOURCES c++/*.cpp)

file(GLOB ${PROJECT_NAME}_UIS ui/*.ui)

file(GLOB ${PROJECT_NAME}_RESOURCES res/*)
list(REMOVE_ITEM ${PROJECT_NAME}_RESOURCES "${PROJECT_SOURCE_DIR}/res/descriptor.desktop.in")

include_directories(${INTERFACES_DIR})
include_directories(${CMAKE_BINARY_DIR})

## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

## Qt
find_package(Qt5Core        REQUIRED)
find_package(Qt5Gui         REQUIRED)
find_package(Qt5Widgets     REQUIRED)


## LXQT -- depends on QT5XDG
find_package(LXQT REQUIRED )
include(${LXQT_USE_FILE})

## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_WRAP_UI(${PROJECT_NAME}_UI_HEADERS ${${PROJECT_NAME}_UIS})
QT5_ADD_RESOURCES(${PROJECT_NAME}_QRC_SOURCES ${${PROJECT_NAME}_RESOURCES})


## Inter-module dependencies
set(${PROJECT_NAME}_DEPENDS )
## --------------------------------------------------------------
## Build & Link
## --------------------------------------------------------------
set (${PROJECT_NAME}_SOURCES
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_UI_HEADERS}
    ${${PROJECT_NAME}_QRC_SOURCES}
)

m_build_module(
    NAME ${PROJECT_NAME}
    SOURCES ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${TARGET_NAME}
    ${LXQT_LIBRARIES}

    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

m_deploy_module(${TARGET_NAME})