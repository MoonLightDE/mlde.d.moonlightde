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
set(MODULE_NAME core_test)
set(MODULE_FULL_NAME "Core Test module")
project(moonlightDE-${MODULE_NAME})

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
set(${PROJECT_NAME}_HEADERS
    c++/TestICore.h    
)
set(${PROJECT_NAME}_PUBLIC_HEADERS
)

set(${PROJECT_NAME}_SOURCES 
    c++/Activator.cpp
    c++/TestICore.cpp
)

set ( ${PROJEC_NAME}_UIS
)

set ( ${PROJEC_NAME}_RESOURCES
)

## --------------------------------------------------------------
## Dependencies - Qt
## -------------------------------- ------------------------------
find_package(Qt5Core    REQUIRED)
find_package(Qt5Gui     REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_WRAP_UI(${PROJEC_NAME}_UI_HEADERS ${${PROJEC_NAME}_UIS})
QT5_ADD_RESOURCES(${PROJEC_NAME}_QRC_SOURCES ${${PROJEC_NAME}_RESOURCES})

## --------------------------------------------------------------
## Dependencies - internal
## --------------------------------------------------------------
include_directories(${INTERFACES_DIR}/${MODULE_NAME})
include_directories(${INTERFACES_DIR})

#CppMicroServices
find_package(CppMicroServices NO_MODULE REQUIRED)
include_directories(${CppMicroServices_INCLUDE_DIRS})

include_directories(${CMAKE_BINARY_DIR})

## --------------------------------------------------------------
## Build Rules
## --------------------------------------------------------------

usFunctionGenerateModuleInit( ${PROJECT_NAME}_SOURCES
        NAME ${MODULE_FULL_NAME}
        LIBRARY_NAME "${MODULE_NAME}"
)

set( IDDLE_MODULE_SOURCES
    "c++/IddleModule.cpp"
)
usFunctionGenerateModuleInit( IDDLE_MODULE_SOURCES
        NAME "moonlightDE-iddle_module" 
        LIBRARY_NAME "iddle_module"
)

add_library(${PROJECT_NAME} SHARED 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJEC_NAME}_UI_HEADERS}
    ${${PROJEC_NAME}_QRC_SOURCES}
)

add_library(moonlightDE-iddle_module SHARED 
    ${IDDLE_MODULE_SOURCES}
)

qt5_use_modules(${PROJECT_NAME} 
    Core
    Gui 
    Widgets 
    Test
)

qt5_use_modules(moonlightDE-iddle_module
   Core
)
# Link the CppMicroServices library
target_link_libraries(${PROJECT_NAME} ${CppMicroServices_LIBRARIES} )

target_link_libraries(moonlightDE-iddle_module ${CppMicroServices_LIBRARIES} )

## --------------------------------------------------------------
## Install
## --------------------------------------------------------------
INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${MODULES_OUTPUT_DIR}
    ARCHIVE DESTINATION lib/static)

INSTALL(FILES ${${PROJECT_NAME}_PUBLIC_HEADERS} DESTINATION include/${MoonLightDEPrefix} )
