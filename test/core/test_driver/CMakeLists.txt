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
project(test_driver)

## -----------------------------------------------------------------------------
## Sources
## -----------------------------------------------------------------------------
set(${PROJECT_NAME}_PUBLIC_HEADERS
)

set(${PROJECT_NAME}_SOURCES 
    Activator.cpp
    TestICore.h    
    TestICore.cpp
)

set (${PROJECT_NAME}_UIS
)

set (${PROJECT_NAME}_RESOURCES
)

set( ${PROJECT_NAME}_DESCRIPTOR 
    descriptor.in
)

## -----------------------------------------------------------------------------
## Dependencies - Qt
## -------------------------------- --------------------------------------------
find_package(Qt5Core    REQUIRED)
find_package(Qt5Test REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_WRAP_UI(${PROJECT_NAME}_UI_HEADERS ${${PROJECT_NAME}_UIS})
QT5_ADD_RESOURCES(${PROJECT_NAME}_QRC_SOURCES ${${PROJECT_NAME}_RESOURCES})



## -----------------------------------------------------------------------------
## Build Rules
## -----------------------------------------------------------------------------

MoonLightDE_CreateModule(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

qt5_use_modules(moonlightDE-${PROJECT_NAME} 
    Core
    Test
)