#    Copyright (C) 2014 Moonlight Desktop Environment Team
#    Authors:
#       Alexis López Zubieta
#
#    This file is part of the Moonlight Desktop Environment and it's based on
#    the original pcmanfm-qt build script.
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

set(MODULE_NAME File_Manager)
set(MODULE_FULL_NAME "File_Manager")
project(moonlightDE-${MODULE_NAME})

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------
include_directories(
  ${QT_INCLUDES}
  ${LIBFM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ../libfm-qt
)

link_directories(
  ${LIBFM_LIBRARY_DIRS}
)

set(pcmanfm_SRCS
  Activator.cpp
  application.cpp
#  pcmanfm.cpp
  mainwindow.cpp
  tabpage.cpp
  view.cpp
  launcher.cpp
  preferencesdialog.cpp
  desktoppreferencesdialog.cpp
  desktopwindow.cpp
  desktopitemdelegate.cpp
  autorundialog.cpp
  settings.cpp
)

set(pcmanfm_UIS
  main-win.ui
  about.ui
  preferences.ui
  desktop-preferences.ui
  autorun.ui
)

## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

## CppMicroServices
find_package(CppMicroServices NO_MODULE REQUIRED)
include_directories(${CppMicroServices_INCLUDE_DIRS})

## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------

usFunctionGenerateModuleInit( pcmanfm_SRCS
        NAME ${MODULE_FULL_NAME}
        LIBRARY_NAME "${MODULE_NAME}"
)


if(USE_QT5)
  qt5_add_dbus_adaptor(pcmanfm_SRCS
    org.pcmanfm.Application.xml
    application.h
    PCManFM::Application
    applicationadaptor
    ApplicationAdaptor
  )
else()
  qt4_add_dbus_adaptor(pcmanfm_SRCS
    org.pcmanfm.Application.xml
    application.h
    PCManFM::Application
    applicationadaptor
    ApplicationAdaptor
  )
endif()


if(USE_QT5)
  qt5_wrap_ui(pcmanfm_UIS_H ${pcmanfm_UIS})
else()
  qt4_wrap_ui(pcmanfm_UIS_H ${pcmanfm_UIS})
endif()



# add translation for pcmanfm-qt
# See http://www.cmake.org/Wiki/CMake:How_To_Build_Qt4_Software
file(GLOB TS_FILES translations/*.ts)

if(USE_QT5)
  qt5_create_translation(QM_FILES ${pcmanfm_SRCS} ${pcmanfm_UIS_H} ${TS_FILES})
  qt5_add_translation(QM_FILES ${TS_FILES})
else()
  qt4_create_translation(QM_FILES ${pcmanfm_SRCS} ${pcmanfm_UIS_H} ${TS_FILES})
  qt4_add_translation(QM_FILES ${TS_FILES})
endif()


# prevent the generated files from being deleted during make clean
set_directory_properties(PROPERTIES
  CLEAN_NO_CUSTOM true
)



## --------------------------------------------------------------
## Build & Link
## --------------------------------------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${MoonLightDEPrefix}/modules)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED 
  ${pcmanfm_SRCS}
  ${pcmanfm_UIS_H}
)
set_property(
   TARGET ${PROJECT_NAME} APPEND
   PROPERTY COMPILE_DEFINITIONS
   LIBFM_QT_API=Q_DECL_IMPORT
   PCMANFM_DATA_DIR="${CMAKE_INSTALL_PREFIX}/share/pcmanfm-qt"
)
target_link_libraries(${PROJECT_NAME} 
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${LIBFM_LIBRARIES}
  fm-qt
)

if(USE_QT5)
  qt5_use_modules(${PROJECT_NAME} Widgets DBus)
endif()

add_custom_target (pcmanfm_translations DEPENDS ${QM_FILES})

## --------------------------------------------------------------
## Deploy
## --------------------------------------------------------------

#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${MODULES_OUTPUT_DIR}
    ARCHIVE DESTINATION lib/static)


# install a desktop entry file for pcmanfm-qt and desktop preferences
# it's no longer an application only preferences will be installed
#configure_file(pcmanfm-qt.desktop.in pcmanfm-qt.desktop @ONLY) 
configure_file(pcmanfm-qt-desktop-pref.desktop.in pcmanfm-qt-desktop-pref.desktop @ONLY)
install(
  FILES # ${CMAKE_CURRENT_BINARY_DIR}/pcmanfm-qt.desktop
        ${CMAKE_CURRENT_BINARY_DIR}/pcmanfm-qt-desktop-pref.desktop
  DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/pcmanfm-qt/translations)
