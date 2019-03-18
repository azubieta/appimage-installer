function(configure_module target_name)
    target_include_directories(
        ${target_name}

        PUBLIC ${PROJECT_SOURCE_DIR}/src
        PUBLIC ${libappimage_INCLUDE_DIRS}
        PUBLIC ${libappimage_INCLUDE_DIRS}
        PUBLIC $<TARGET_PROPERTY:libappimage,INTERFACE_INCLUDE_DIRECTORIES>
        PUBLIC $<TARGET_PROPERTY:Qt5::Core,INTERFACE_INCLUDE_DIRECTORIES>
        PUBLIC $<TARGET_PROPERTY:Qt5::Network,INTERFACE_INCLUDE_DIRECTORIES>
        #    PUBLIC $<TARGET_PROPERTY,INTERFACE_INCLUDE_DIRECTOIES:Qt5::Core>
    )

    set_property(TARGET ${target_name} PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_compile_definitions(${target_name} PRIVATE LIBAPPIMAGE_DESKTOP_INTEGRATION_ENABLED)
    target_compile_definitions(${target_name} PRIVATE LIBAPPIMAGE_THUMBNAILER_ENABLED)
endfunction()
