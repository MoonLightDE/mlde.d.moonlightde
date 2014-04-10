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
set(MODULE_NAME core)
project(moonlightDE${MODULE_NAME})

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
set(${PROJECT_NAME}_HEADERS
    c++/Controller.h
    c++/SettingsProfile.h
    c++/ModuleManager.h
    c++/Environment.h
)
set(${PROJECT_NAME}_PUBLIC_HEADERS
    ${INTERFACES_DIR}/core/ICore.h

)

set(${PROJECT_NAME}_SOURCES
    c++/Controller.cpp
    c++/SettingsProfile.cpp
    c++/ModuleManager.cpp
    c++/Environment.cpp
    c++/main.cpp
)

set ( ${PROJEC_NAME}_UIS
)

set ( ${PROJEC_NAME}_RESOURCES
)

## --------------------------------------------------------------
## Dependencies - Qt
## --------------------------------------------------------------
find_package(Qt5Core    REQUIRED)
find_package(Qt5Gui     REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_WRAP_UI(${PROJEC_NAME}_UI_HEADERS ${${PROJEC_NAME}_UIS})
QT5_ADD_RESOURCES(${PROJEC_NAME}_QRC_SOURCES ${${PROJEC_NAME}_RESOURCES})

## --------------------------------------------------------------
## Dependencies - internal
## --------------------------------------------------------------
include_directories(${INTERFACES_DIR})


#CppMicroServices
find_package(CppMicroServices NO_MODULE REQUIRED)
include_directories(${CppMicroServices_INCLUDE_DIRS})

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR}/core-module/settings-manager)

## --------------------------------------------------------------
## Build Rules
## --------------------------------------------------------------

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${MoonLightDEPrefix})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


usFunctionGenerateExecutableInit(${PROJECT_NAME}_SOURCES
                                 IDENTIFIER "MoonLightDECore"
                                 )


configure_file( c++/MoonLightDECoreConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/MoonLightDECoreConfig.h)

add_executable(${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJEC_NAME}_UI_HEADERS}
    ${${PROJEC_NAME}_QRC_SOURCES}
)

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Gui)
qt5_use_modules(${PROJECT_NAME} Widgets)

# Link the CppMicroServices library
target_link_libraries(${PROJECT_NAME} CppMicroServices )

## --------------------------------------------------------------
## Install
## --------------------------------------------------------------


INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION MODULES_OUTPUT_DIR
    ARCHIVE DESTINATION lib/static)

INSTALL(FILES ${${PROJECT_NAME}_PUBLIC_HEADERS} DESTINATION ${INTERFACES_OUTPUT_DIR}/${MODULE_NAME} )
