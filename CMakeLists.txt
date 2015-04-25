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

project(moonlightde) # The project name will be prefixed all targets.

set(${PROJECT_NAME}_MAJOR_VERSION "0")
set(${PROJECT_NAME}_MINOR_VERSION "7")
set(${PROJECT_NAME}_PATCH_VERSION "0")

set(${PROJECT_NAME}_VERSION '${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION}')

set(LIBS_PREFIX "lib${PROJECT_NAME}_")
# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)

## C++ 11 Required
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



## Build Type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -gdwarf-2")
    message(STATUS "Using CXX_FLAGS: ${CMAKE_CXX_FLAGS_DEBUG}")

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
        STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Build config: ${PROJECT_NAME} ${${PROJECT_NAME}_VERSION} ${CMAKE_BUILD_TYPE}")

#-------------------------------------------------------------------------------
# Default install directories
#-------------------------------------------------------------------------------

set(LIBS_INSTALL_DIR "lib/${CMAKE_PROJECT_NAME}/")
set(MODULES_INSTALL_DIR "lib/${CMAKE_PROJECT_NAME}/modules")
set(DESCRIPTORS_INSTALL_DIR "lib/${CMAKE_PROJECT_NAME}/descriptors")
set(RESOURCES_INSTALL_DIR "share/${CMAKE_PROJECT_NAME}")
set(PUBLIC_HEADERS_INSTALL_DIR "include/${CMAKE_PROJECT_NAME}")
set(CMAKE_FILES_INSTALL_DIR "share/${CMAKE_PROJECT_NAME}/cmake")

## --------------------------------------------------------------
## MoonlightDE modules,  comment to disable build.
## --------------------------------------------------------------
include(src/utils_cmake/build-module.cmake)  # every module depends on this, don't disable!
include(src/utils_cmake/deploy-module.cmake) # every module depends on this, don't disable!

add_subdirectory(src/core)

add_subdirectory(src/utils_module_skel)

add_subdirectory(src/presentation_panel)
add_subdirectory(src/presentation_panel_dash)
add_subdirectory(src/presentation_panel_tasksbar)
add_subdirectory(src/presentation_panel_systray)
add_subdirectory(src/presentation_panel_datetime)
add_subdirectory(src/presentation_panel_quicklaunchers)

add_subdirectory(src/model_theme_manager)

add_subdirectory(src/model_filesystem_gvfs)
add_subdirectory(src/presentation_filemanager)

add_subdirectory(src/model_notifications)
add_subdirectory(src/presentation_notifiactions_box)

#add_subdirectory(src/panel-clasic-menu)
#add_subdirectory(src/panel-dash_xdg)
#
#add_subdirectory(src/panel-menu)

#add_subdirectory(src/panel-quicklauchers)
#
#add_subdirectory(src/actions_registry)
#add_subdirectory(src/keyboard-shortcuts)
#add_subdirectory(src/notifications)
#add_subdirectory(src/file-manager)
#add_subdirectory(src/runner)

#add_subdirectory(src/remote)

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
