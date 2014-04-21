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

## --------------------------------------------------------------
## Configure Build
## --------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
set(INTERFACES_DIR ${CMAKE_SOURCE_DIR}/include)

set(MOONLIGHTDE_PREFIX MoonLightDE)
set(MODULES_OUTPUT_DIR ${CMAKE_INSTALL_PREFIX}/lib/${MOONLIGHTDE_PREFIX}/modules )
set(INTERFACES_OUTPUT_DIR ${CMAKE_INSTALL_PREFIX}/include/${MOONLIGHTDE_PREFIX})

## --------------------------------------------------------------
## Modules
## --------------------------------------------------------------
add_subdirectory(src/core)

add_subdirectory(src/panel)

add_subdirectory(src/actions_registry)
## --------------------------------------------------------------
## Third Parthy
## --------------------------------------------------------------
