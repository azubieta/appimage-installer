# Common settings module
add_library(settings-common OBJECT Settings.cpp Settings.h)

configure_module(settings-common)
target_include_directories(settings-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_SETTINGS_DIALOG)
    add_executable(
        appimage-cli-tool-settings
        main.cpp
        SettingsDialog.h
        SettingsDialog.cpp
        SettingsDialog.ui

        $<TARGET_OBJECTS:settings-common>
    )

    configure_module(appimage-cli-tool-settings)
    target_link_libraries(appimage-cli-tool-settings PRIVATE Qt5::Core Qt5::Widgets yaml-cpp)

    add_dependencies(appimage-cli-tool-settings settings-common)

    # Install
    install(
        TARGETS appimage-cli-tool-settings
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT APPIMAGE_CLI_TOOL_SETTINGS
    )
endif()
