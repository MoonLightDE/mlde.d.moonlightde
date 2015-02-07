option(GENERATE_MODULE_CONFIGS "Complete missing module parts." Off)

# Description: Deploys MoonLightDE modules at the right location.
# 
# Input: Module name
# Output: 
#
#
function(m_deploy_module TARGET_NAME)
    INSTALL(TARGETS ${TARGET_NAME}
        LIBRARY DESTINATION ${MODULES_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION ${PUBLIC_HEADERS_INSTALL_DIR}/${PROJECT_NAME}
    )

    get_target_property(DESCRIPTOR ${TARGET_NAME} DESCRIPTOR)    
    if (DESCRIPTOR)
        INSTALL(FILES ${DESCRIPTOR} DESTINATION ${DESCRIPTORS_INSTALL_DIR})
    endif()
    
    get_target_property(RESOURCES ${TARGET_NAME} "RESOURCES")
    if (RESOURCES)
        INSTALL(FILES ${RESOURCES} DESTINATION ${RESOURCES_INSTALL_DIR}/${PROJECT_NAME})
    endif()

endfunction()



