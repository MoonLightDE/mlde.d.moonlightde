# Create a module for moonlightDE using LibCppMicroServices. The resulting 
# target name is formed by the MoonLightDE prefix and the module name.
# usage example:
#	m_create_module(
# 		NAME "themeManager"
#		DESCRIPTOR "themeManager.desktop.in" # Optional
#		INTERFACES "../include"              # Optional
#		SOURCES "activator.cpp ThemeManager.cpp"
#		DEPENCENCIES ""                      # Optional
#	)
#

function(m_create_module)
    include (CMakeParseArguments)
    cmake_parse_arguments(MOONLIGHTDE_MODULE "" "NAME;DESCRIPTOR" "INTERFACES;SOURCES;DEPENDENCIES" ${ARGN})
	
    # Validate arguments
    if(NOT MOONLIGHTDE_MODULE_NAME)
            message(FATAL_ERROR "NAME argument is mandatory.")
    endif()
    if ( NOT MOONLIGHTDE_MODULE_SOURCES )
            message(FATAL_ERROR "SOURCES argument is mandatory.")
    endif()

    # Ensure CppMicroServices is present
    find_package(CppMicroServices NO_MODULE REQUIRED)
    include_directories(${CppMicroServices_INCLUDE_DIRS})

    usFunctionGenerateModuleInit(MOONLIGHTDE_MODULE_SOURCES
        NAME ${MOONLIGHTDE_MODULE_NAME}
        LIBRARY_NAME ${MOONLIGHTDE_MODULE_NAME}
    )
    
    add_library(${MOONLIGHTDE_PREFIX}_${MOONLIGHTDE_MODULE_NAME} SHARED ${MOONLIGHTDE_MODULE_SOURCES})
    
    target_include_directories(${MOONLIGHTDE_PREFIX}_${MOONLIGHTDE_MODULE_NAME} PUBLIC  
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/${MOONLIGHTDE_MODULE_NAME}>  
        $<INSTALL_INTERFACE:${PUBLIC_HEADERS_INSTALL_DIR}/${MOONLIGHTDE_MODULE_NAME}>  
    )

    target_link_libraries(${MOONLIGHTDE_PREFIX}_${MOONLIGHTDE_MODULE_NAME}
	${CppMicroServices_LIBRARIES}
	${MOONLIGHTDE_MODULE_DEPENDENCIES}
    )

    set_target_properties(${MOONLIGHTDE_PREFIX}_${MOONLIGHTDE_MODULE_NAME} 
        PROPERTIES
        PUBLIC_HEADER "${MOONLIGHTDE_MODULE_INTERFACES}"
        DESCRIPTOR "${MOONLIGHTDE_MODULE_DESCRIPTOR}"
    )

    # Create module configuration header
    if(NOT EXISTS "module_config.h" )
        file(WRITE "${PROJECT_BINARY_DIR}/module_config.h"
            "#ifndef ${MOONLIGHTDE_MODULE_NAME}_H\n"
            "#define ${MOONLIGHTDE_MODULE_NAME}_H\n"
            
            "#define MODULE_NAME \"${MOONLIGHTDE_MODULE_NAME}\" \n"

            "#endif // ${MOONLIGHTDE_MODULE_NAME}_H\n"
            )
    endif()

    # Build module descriptor
    if(NOT EXISTS ${MOONLIGHTDE_MODULE_DESCRIPTOR})
        message(STATUS "No module descriptor found for ${MOONLIGHTDE_MODULE_NAME}, generating a new one.")
        file(WRITE ${MOONLIGHTDE_MODULE_NAME}.desktop.in
            "[Desktop Entry]\n"
            "Type=MoonLight-Module\n"
            "Name=${MOONLIGHTDE_MODULE_NAME}\n"
            "Comment=No description privided.\n")
            
        set(MOONLIGHTDE_MODULE_DESCRIPTOR "${MOONLIGHTDE_MODULE_NAME}.desktop.in")
    endif()
    configure_file(
		"${PROJECT_SOURCE_DIR}/${MOONLIGHTDE_MODULE_DESCRIPTOR}"
		"${PROJECT_BINARY_DIR}/${MOONLIGHTDE_MODULE_NAME}.desktop" )

    ## Install
    # lib, headers, resources
    install(TARGETS ${MOONLIGHTDE_PREFIX}_${MOONLIGHTDE_MODULE_NAME} 
        EXPORT ${MOONLIGHTDE_MODULE_NAME} DESTINATION ${MODULES_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION ${PUBLIC_HEADERS_INSTALL_DIR}
        RESOURCE DESTINATION ${RESOURCES_INSTALL_DIR}/${MOONLIGHTDE_MODULE_NAME}
    )

    # descriptor
    install(FILES ${MOONLIGHTDE_MODULE_DESCRIPTOR} DESTINATION ${DESCRIPTORS_INSTALL_DIR} )

    # cmake find module files
    install(EXPORT ${MOONLIGHTDE_MODULE_NAME} DESTINATION ${CMAKE_FILES_INSTALL_DIR})
endfunction()
