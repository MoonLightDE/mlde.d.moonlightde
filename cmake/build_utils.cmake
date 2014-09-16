find_package(CppMicroServices NO_MODULE REQUIRED)
include_directories(${CppMicroServices_INCLUDE_DIRS})


function(MoonLightDE_CreateModule _name)
    usFunctionGenerateModuleInit( ${_name}_SOURCES
        NAME moonlightDE-${_name}
        LIBRARY_NAME ${_name}
    )

    if(EXISTS "descriptor.in")
        configure_file( descriptor.in ${MoonLightDE_DESCRIPTORS_OUTPUT_DIRECTORY}/moonlightDE-${_name}.desktop)
    else()
        message(STATUS "No module descriptor found for ${_name}, generating new one.")
        file(WRITE ${MoonLightDE_DESCRIPTORS_OUTPUT_DIRECTORY}/moonlightDE-${_name}.desktop
            "[Desktop Entry]\n"
            "Type=MoonLight-Module\n"
            "Name=${_name}\n"
            "Comment=No description privided.\n")
    endif()
    
    add_library(moonlightDE-${_name} SHARED ${ARGN} ${${_name}_SOURCES})
    
    if(${_name}_DEPENDS)
        foreach(_dep ${${_name}_DEPENDS})
          include_directories(${MoonLightDE_HEADERS_OUTPUT_DIRECTORY}/${_dep})
          target_link_libraries(moonlightDE-${_name} ${MoonLightDE_LIBRARY_OUTPUT_DIRECTORY}/moonlightDE-${_dep})
        endforeach()
    endif()
    target_link_libraries(moonlightDE-${_name} ${CppMicroServices_LIBRARIES})
    

    INSTALL(TARGETS moonlightDE-${_name}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/${_name}
        ARCHIVE DESTINATION lib/static
    )

    INSTALL(FILES ${${_name}_PUBLIC_HEADERS} DESTINATION include/moonlightDE/${_name} )
    INSTALL(FILES ${${MoonLightDE_DESCRIPTORS_OUTPUT_DIRECTORY}/moonlightDE-${_name}.desktop} DESTINATION share/moonlightDE/descriptors )
endfunction()