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
	cmake_parse_arguments(MOONLIGHTDE_MODULE "" "NAME;DESCRIPTOR;INTERFACES" "SOURCES;DEPENDENCIES" ${ARGN})
	
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
    
    target_include_directories(${MOONLIGHTDE_PREFIX}_${MOONLIGHTDE_MODULE_NAME} PUBLIC ${MOONLIGHTDE_MODULE_INTERFACES})
    target_link_libraries(${MOONLIGHTDE_PREFIX}_${MOONLIGHTDE_MODULE_NAME}
	${CppMicroServices_LIBRARIES}
	${MOONLIGHTDE_MODULE_DEPENDENCIES}
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

    # Install 
    #~ INSTALL(TARGETS moonlightDE-${_name}
        #~ RUNTIME DESTINATION bin
        #~ LIBRARY DESTINATION lib/${_name}
        #~ ARCHIVE DESTINATION lib/static
        #~ PUBLIC_HEADER include/${MOONLIGHTDE_MODULE_NAME}
    #~ )
#~ 
    #~ INSTALL(FILES ${${_name}_PUBLIC_HEADERS} DESTINATION include/moonlightDE/${_name} )
    #~ INSTALL(FILES ${${MoonLightDE_DESCRIPTORS_OUTPUT_DIRECTORY}/moonlightDE-${_name}.desktop} DESTINATION share/moonlightDE/descriptors )
endfunction()
