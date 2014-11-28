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

project(moonlightDE)
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 7)
set(${PROJECT_NAME}_PATCH_VERSION 0)

set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION})

# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)

## Build Type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -gdwarf-2")
    message(STATUS "Using CXX_FLAGS: ${CMAKE_CXX_FLAGS_DEBUG}")

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
        STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#-------------------------------------------------------------------------------
# Init output directories
#-------------------------------------------------------------------------------

set(MoonLightDE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(MoonLightDE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(MoonLightDE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(MoonLightDE_HEADERS_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")
set(MoonLightDE_DESCRIPTORS_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/share/moonlightDE/descriptors")

set(DBUS_INTERFACES_INSTALL_DIR "share/dbus-1/interfaces")

foreach(_type ARCHIVE LIBRARY RUNTIME HEADERS)
  if(NOT CMAKE_${_type}_OUTPUT_DIRECTORY)
    set(CMAKE_${_type}_OUTPUT_DIRECTORY ${MoonLightDE_${_type}_OUTPUT_DIRECTORY})
  endif()
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/include)

include(cmake/build_utils.cmake)
## --------------------------------------------------------------
## Modules
## --------------------------------------------------------------
add_subdirectory(src/core)

option(TEST_MODE "Build test modules?" OFF)
if (TEST_MODE)
    add_subdirectory(test/core)
endif(TEST_MODE)



add_subdirectory(src/panel)
#add_subdirectory(src/panel-clasic-menu)
add_subdirectory(src/panel-dash_xdg)
#
add_subdirectory(src/panel-clock)
#add_subdirectory(src/panel-menu)
add_subdirectory(src/panel-taskbar)
add_subdirectory(src/panel-systray)
#add_subdirectory(src/panel-quicklauchers)
#
#add_subdirectory(src/actions_registry)
#add_subdirectory(src/keyboard-shortcuts)
add_subdirectory(src/notifications)
#add_subdirectory(src/file-manager)
#add_subdirectory(src/runner)

add_subdirectory(src/remote)

## --------------------------------------------------------------
## Third Parthy
## --------------------------------------------------------------


# ------------------------------------------------------------------------------
# Distribution
# ------------------------------------------------------------------------------

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MoonLight DE")
SET(CPACK_PACKAGE_VENDOR "")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR_VERSION} )
SET(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_MINOR_VERSION} )
SET(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_PATCH_VERSION} )
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

SET(CPACK_SET_DESTDIR "ON")

## Debian especific packing instructions
SET(CPACK_PACKAGE_CONTACT "MoonLightDE Mail List <moonlightde@googlegroups.com>")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libcppmicroservices" "qt5-defaults" "libqt5x11extras5")

SET(CPACK_PACKAGE_EXECUTABLES "bin/moonlight-core")
INCLUDE(CPack)
