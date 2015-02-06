# Description: Create a module for moonlightDE using LibCppMicroServices. The 
# resulting target name is formed by the MoonLightDE prefix and the module name.
# Input: 
#   NAME: module name
#   SOURCES: ...
#
# Ouput:
#   TARGET_NAME : Name of the build target resulting from the join of the 
#       meta_project name and the module name. Use it for linking.
#   TARGET_DESCRIPTOR: Path to the module descriptor file.
#
# usage example:
#	m_create_module(
# 		NAME "themeManager"
#		SOURCES "activator.cpp ThemeManager.cpp"
#	)
#
function(m_build_module)
    include (CMakeParseArguments)
    cmake_parse_arguments(MOONLIGHTDE_MODULE "" "NAME" "SOURCES;DEPENDENCIES" ${ARGN})
	
    # Validate arguments
    if(NOT MOONLIGHTDE_MODULE_NAME)
            message(FATAL_ERROR "NAME argument is mandatory.")
    endif()
    if ( NOT MOONLIGHTDE_MODULE_SOURCES )
            message(FATAL_ERROR "SOURCES argument is mandatory.")
    endif()

    set(MODULE_FULL_NAME "${CMAKE_PROJECT_NAME}_${MOONLIGHTDE_MODULE_NAME}")
    message(STATUS "Building module lib: ${MODULE_FULL_NAME}")

    # Create module configuration header
    set(MODULE_CONFIGURATION_HEADER_TEMPLATE "${PROJECT_SOURCE_DIR}/c++/module_config.h.in")
    if(NOT EXISTS "${MODULE_CONFIGURATION_HEADER_TEMPLATE}" )
        message(AUTHOR_WARNING "Generating module configuration template at: "
                "${MODULE_CONFIGURATION_HEADER_TEMPLATE}" )

        file(WRITE "${MODULE_CONFIGURATION_HEADER_TEMPLATE}"
            "#ifndef ${MOONLIGHTDE_MODULE_NAME}_H\n"
            "#define ${MOONLIGHTDE_MODULE_NAME}_H\n"
            
            "#define MODULE_NAME \"${MOONLIGHTDE_MODULE_NAME}\" \n"

            "#endif // ${MOONLIGHTDE_MODULE_NAME}_H\n"
            )
    endif()
    configure_file(
        "${MODULE_CONFIGURATION_HEADER_TEMPLATE}"
	"${PROJECT_BINARY_DIR}/module_config.h"
    )
    
    # Ensure that the header created is reachable
    include_directories(${CMAKE_BINARY_DIR})

    # Ensure other modules headers are available
    include_directories(${CMAKE_SOURCE_DIR}/include)

    ##
    ## CppMicroServices preprosecing
    ##
    find_package(CppMicroServices NO_MODULE REQUIRED)
    include_directories(${CppMicroServices_INCLUDE_DIRS})

    usFunctionGenerateModuleInit(MOONLIGHTDE_MODULE_SOURCES
        NAME ${MOONLIGHTDE_MODULE_NAME}
        LIBRARY_NAME ${MOONLIGHTDE_MODULE_NAME}
    )

    add_library(${MODULE_FULL_NAME} SHARED ${MOONLIGHTDE_MODULE_SOURCES})

    ## Include public headers
    if (EXISTS ${CMAKE_SOURCE_DIR}/include/${MOONLIGHTDE_MODULE_NAME})
        target_include_directories(${MODULE_FULL_NAME} PUBLIC  
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> 
            $<INSTALL_INTERFACE:${PUBLIC_HEADERS_INSTALL_DIR}>  
        )
    else()
        message(AUTHOR_WARNING "${MOONLIGHTDE_MODULE_NAME} public headers were not found at: "
                "${CMAKE_SOURCE_DIR}/include/${MOONLIGHTDE_MODULE_NAME}")
    endif()
    
    target_link_libraries(${MODULE_FULL_NAME}  ${CppMicroServices_LIBRARIES} )

    # Build module descriptor
    set(MODULE_DESCRIPTOR_TEMPLATE "${PROJECT_SOURCE_DIR}/res/descriptor.desktop.in" )

    if(NOT EXISTS ${MODULE_DESCRIPTOR_TEMPLATE})
        message(STATUS "No module descriptor found at ${MODULE_DESCRIPTOR_TEMPLATE}, generating a new one.")
        m_generate_module_descriptor_template(${MODULE_FULL_NAME}  ${MODULE_DESCRIPTOR_TEMPLATE})
    endif()
    
    set(MODULE_DESCRIPTOR "${PROJECT_BINARY_DIR}/${MOONLIGHTDE_MODULE_NAME}.desktop")    
    configure_file( ${MODULE_DESCRIPTOR_TEMPLATE} ${MODULE_DESCRIPTOR} )
    
    file(GLOB PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/include/${MOONLIGHTDE_MODULE_NAME}/*.h)
    set_target_properties(${MODULE_FULL_NAME} 
        PROPERTIES
        PUBLIC_HEADER "${PUBLIC_HEADERS}"
        DESCRIPTOR "${MODULE_DESCRIPTOR}"
    )

    set(TARGET_NAME "${MODULE_FULL_NAME}" PARENT_SCOPE)
endfunction()

function(m_generate_module_descriptor_template MODULE_NAME TARGET)
    file(WRITE ${TARGET}
        "[Desktop Entry]\n"
        "Type=${CMAKE_PROJECT_NAME}_generic\n"
        "Name=${MODULE_NAME}\n"
        "Comment=No description privided.\n")
endfunction()
