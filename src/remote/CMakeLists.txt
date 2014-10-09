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

project(remote)

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
SET(${PROJECT_NAME}_SOURCES
    c++/Activator.cpp
    c++/Remote.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/org.moonlightde.remote.xml
)

SET(CLI_SOURCES
    c++/cli-main.cpp
)


## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

## Qt
find_package(Qt5Core        REQUIRED)
find_package(Qt5Widgets     REQUIRED)
find_package(Qt5DBus        REQUIRED)


## CppMicroServices
find_package(CppMicroServices NO_MODULE REQUIRED)
include_directories(${CppMicroServices_INCLUDE_DIRS})

## Inter-module dependencies
set(${PROJECT_NAME}_DEPENDS )

## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------

qt5_generate_dbus_interface(
    c++/Remote.h
    ${CMAKE_CURRENT_BINARY_DIR}/org.moonlightde.remote.xml
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.moonlightde.remote.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})


qt5_add_dbus_adaptor(${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/org.moonlightde.remote.xml
                     c++/Remote.h Remote)

qt5_add_dbus_interface(CLI_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/org.moonlightde.remote.xml RemoteClient )

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

## --------------------------------------------------------------
## Build
## --------------------------------------------------------------

MoonLightDE_CreateModule(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(moonlightDE-${PROJECT_NAME} 
    Qt5::Core
    Qt5::DBus
)


add_executable(moonlightDE-cli ${CLI_SOURCES})
target_link_libraries( moonlightDE-cli
    Qt5::Core
    Qt5::DBus
    Qt5::Widgets
)