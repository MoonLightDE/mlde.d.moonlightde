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

set(MODULE_NAME shortcuts_services)
set(MODULE_FULL_NAME "MoonLightDE shortcuts service.")
project(moonlightDE-${MODULE_NAME})

## --------------------------------------------------------------
## Sources
## --------------------------------------------------------------

set(${PROJECT_NAME}_SOURCES
	Activator.cpp
        inner_action.cpp
#        main.cpp
	core.cpp
	daemon_adaptor.cpp
	native_adaptor.cpp
	client_proxy.cpp
	log_target.cpp
	pipe_utils.cpp
	string_utils.cpp
	base_action.cpp
	method_action.cpp
	client_action.cpp
	command_action.cpp
	meta_types.cpp
)

set(${PROJECT_NAME}_CPP_HEADERS
        inner_action.h
	log_target.h
	pipe_utils.h
	string_utils.h
	base_action.h
	method_action.h
	client_action.h
	command_action.h
	meta_types.h
)

set(${PROJECT_NAME}_PUBLIC_HEADERS
    ${INTERFACES_DIR}/keyboard-shortcuts/IKeyboardShortCutsService.h
    ${INTERFACES_DIR}/keyboard-shortcuts/shortcut.h
)
set(${PROJECT_NAME}_FORMS
)

set(${PROJECT_NAME}_RESOURCES
)

set(${PROJECT_NAME}_TRANSLATIONS
)

set(${PROJECT_NAME}_DBUS_ADAPTORS
	org.lxqt.global_key_shortcuts.daemon.xml
	org.lxqt.global_key_shortcuts.native.xml
)

set_source_files_properties(org.lxqt.global_key_shortcuts.daemon.xml PROPERTIES
	INCLUDE daemon_adaptor.h
	PARENT_CLASSNAME DaemonAdaptor
	BASENAME org.lxqt.global_key_shortcuts.daemon
	CLASSNAME OrgLxqtGlobalActionDaemonAdaptor
)

set_source_files_properties(org.lxqt.global_key_shortcuts.native.xml PROPERTIES
	INCLUDE native_adaptor.h
	PARENT_CLASSNAME NativeAdaptor
	BASENAME org.lxqt.global_key_shortcuts.native
	CLASSNAME OrgLxqtGlobalActionNativeAdaptor
)

set(${PROJECT_NAME}_DBUS_INTERFACES
	org.lxqt.global_key_shortcuts.client.xml
)

set_source_files_properties(org.lxqt.global_key_shortcuts.client.xml PROPERTIES
	BASENAME org.lxqt.global_key_shortcuts.client
)

set(${PROJECT_NAME}_HEADERS
	${${PROJECT_NAME}_CPP_HEADERS}
        ${${PROJECT_NAME}_PUBLIC_HEADERS}
)

set(${PROJECT_NAME}_TRANSLATABLE
	${${PROJECT_NAME}_SOURCES}
	${${PROJECT_NAME}_HEADERS}
	${${PROJECT_NAME}_FORMS}
)

## --------------------------------------------------------------
## Dependencies
## -------------------------------- ------------------------------

## Qt5
cmake_minimum_required(VERSION 2.8.11)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5LinguistTools REQUIRED QUIET)

## CppMicroServices
find_package(CppMicroServices NO_MODULE REQUIRED)
include_directories(${CppMicroServices_INCLUDE_DIRS})

## LXQT
find_package(lxqt-qt5 REQUIRED QUIET)
include(${LXQT_USE_FILE})

## X11
find_package(X11)
include_directories(${X11_INCLUDE_DIR})


## --------------------------------------------------------------
## Preprocessing
## --------------------------------------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)


usFunctionGenerateModuleInit( ${PROJECT_NAME}_SOURCES
        NAME ${MODULE_FULL_NAME}
        LIBRARY_NAME "${MODULE_NAME}"
)

set(QTX_INCLUDE_DIRS "")
set(QTX_LIBRARIES Qt5::Widgets Qt5::DBus Qt5::Core)

set(QT_DBUS_PREFIX "org.qtproject")

include_directories(
	${PROJECT_SOURCE_DIR}
	${QTX_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_BINARY_DIR}

        ${INTERFACES_DIR}
)

foreach(DBUS_ADAPTOR ${${PROJECT_NAME}_DBUS_ADAPTORS})
	get_filename_component(DBUS_ADAPTOR_FILENAME ${DBUS_ADAPTOR} NAME)
	configure_file(
		${DBUS_ADAPTOR}
		${CMAKE_CURRENT_BINARY_DIR}/${DBUS_ADAPTOR_FILENAME}
		@ONLY
	)
	get_source_file_property(DBUS_ADAPTOR_INCLUDE ${DBUS_ADAPTOR} INCLUDE)
	get_source_file_property(DBUS_ADAPTOR_PARENT_CLASSNAME ${DBUS_ADAPTOR} PARENT_CLASSNAME)
	get_source_file_property(DBUS_ADAPTOR_BASENAME ${DBUS_ADAPTOR} BASENAME)
	get_source_file_property(DBUS_ADAPTOR_CLASSNAME ${DBUS_ADAPTOR} CLASSNAME)
        if(USE_QT5)
                if(DBUS_ADAPTOR_BASENAME)
                        if(DBUS_ADAPTOR_CLASSNAME)
                                qt5_add_dbus_adaptor(${PROJECT_NAME}_DBUS_ADAPTOR_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_ADAPTOR_FILENAME} ${DBUS_ADAPTOR_INCLUDE} ${DBUS_ADAPTOR_PARENT_CLASSNAME} ${DBUS_ADAPTOR_BASENAME} ${DBUS_ADAPTOR_CLASSNAME})
                        else()
                                qt5_add_dbus_adaptor(${PROJECT_NAME}_DBUS_ADAPTOR_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_ADAPTOR_FILENAME} ${DBUS_ADAPTOR_INCLUDE} ${DBUS_ADAPTOR_PARENT_CLASSNAME} ${DBUS_ADAPTOR_BASENAME})
                        endif()
                else()
                        qt5_add_dbus_adaptor(${PROJECT_NAME}_DBUS_ADAPTOR_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_ADAPTOR_FILENAME} ${DBUS_ADAPTOR_INCLUDE} ${DBUS_ADAPTOR_PARENT_CLASSNAME})
                endif()
        else(USE_QT5)
                if(DBUS_ADAPTOR_BASENAME)
                        if(DBUS_ADAPTOR_CLASSNAME)
                                qt4_add_dbus_adaptor(${PROJECT_NAME}_DBUS_ADAPTOR_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_ADAPTOR_FILENAME} ${DBUS_ADAPTOR_INCLUDE} ${DBUS_ADAPTOR_PARENT_CLASSNAME} ${DBUS_ADAPTOR_BASENAME} ${DBUS_ADAPTOR_CLASSNAME})
                        else()
                                qt4_add_dbus_adaptor(${PROJECT_NAME}_DBUS_ADAPTOR_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_ADAPTOR_FILENAME} ${DBUS_ADAPTOR_INCLUDE} ${DBUS_ADAPTOR_PARENT_CLASSNAME} ${DBUS_ADAPTOR_BASENAME})
                        endif()
                else()
                        qt4_add_dbus_adaptor(${PROJECT_NAME}_DBUS_ADAPTOR_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_ADAPTOR_FILENAME} ${DBUS_ADAPTOR_INCLUDE} ${DBUS_ADAPTOR_PARENT_CLASSNAME})
                endif()
        endif(USE_QT5)
endforeach()

foreach(DBUS_INTERFACE ${${PROJECT_NAME}_DBUS_INTERFACES})
	get_filename_component(DBUS_INTERFACE_FILENAME ${DBUS_INTERFACE} NAME)
	configure_file(
		${DBUS_INTERFACE}
		${CMAKE_CURRENT_BINARY_DIR}/${DBUS_INTERFACE_FILENAME}
		@ONLY
	)
	get_source_file_property(DBUS_INTERFACE_BASENAME ${DBUS_INTERFACE} BASENAME)
	get_source_file_property(DBUS_INTERFACE_INCLUDE ${DBUS_INTERFACE} INCLUDE)
	get_source_file_property(DBUS_INTERFACE_CLASSNAME ${DBUS_INTERFACE} CLASSNAME)
	get_source_file_property(DBUS_INTERFACE_NO_NAMESPACE ${DBUS_INTERFACE} NO_NAMESPACE)
	set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${DBUS_INTERFACE_FILENAME} PROPERTIES
		INCLUDE ${DBUS_INTERFACE_INCLUDE}
		CLASSNAME ${DBUS_INTERFACE_CLASSNAME}
		NO_NAMESPACE ${DBUS_INTERFACE_NO_NAMESPACE}
	)
        if(USE_QT5)
                qt5_add_dbus_interface(${PROJECT_NAME}_DBUS_INTERFACE_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_INTERFACE_FILENAME} ${DBUS_INTERFACE_BASENAME})
        else()
                qt4_add_dbus_interface(${PROJECT_NAME}_DBUS_INTERFACE_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_INTERFACE_FILENAME} ${DBUS_INTERFACE_BASENAME})
        endif()
endforeach()


qt5_add_resources(${PROJECT_NAME}_RESOURCE_FILES ${${PROJECT_NAME}_RESOURCES})
qt5_wrap_ui(${PROJECT_NAME}_FORM_FILES ${${PROJECT_NAME}_FORMS})


set(${PROJECT_NAME}_GENERATED_FILES
	${${PROJECT_NAME}_FORM_FILES}
	${${PROJECT_NAME}_RESOURCE_FILES}
	${${PROJECT_NAME}_QM_FILES}
	${${PROJECT_NAME}_DBUS_INTERFACE_FILES}
	${${PROJECT_NAME}_DBUS_ADAPTOR_FILES}
)

set(${PROJECT_NAME}_ALL_FILES
	${${PROJECT_NAME}_SOURCES}
	${${PROJECT_NAME}_HEADERS}
	${${PROJECT_NAME}_GENERATED_FILES}
)

## --------------------------------------------------------------
## Build & Link
## --------------------------------------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${MoonLightDEPrefix}/modules)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED 
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_GENERATED_FILES}
)

#add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_ALL_FILES})
#target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES} ${QTX_LIBRARIES})
#
target_link_libraries(${PROJECT_NAME}
    ${CppMicroServices_LIBRARIES}
    ${X11_LIBRARIES}
    ${QTX_LIBRARIES}
)

## --------------------------------------------------------------
## Deploy
## --------------------------------------------------------------
INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${MODULES_OUTPUT_DIR}
    ARCHIVE DESTINATION lib/static)

INSTALL(FILES ${${PROJECT_NAME}_PUBLIC_HEADERS} DESTINATION include/${MoonLightDEPrefix} )
