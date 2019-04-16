# Common settings module
add_library(settings-common OBJECT Settings.cpp Settings.h)

configure_module(settings-common)
target_include_directories(settings-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Settings Dialog
add_executable(
    appimage-user-tools-settings
    main.cpp
    SettingsDialog.cpp
    SettingsDialog.ui

    $<TARGET_OBJECTS:settings-common>
)

configure_module(appimage-user-tools-settings)
target_link_libraries(appimage-user-tools-settings PRIVATE Qt5::Core Qt5::Widgets yaml-cpp)

add_dependencies(appimage-user-tools-settings settings-common)

# Install
install(
    TARGETS appimage-user-tools-settings
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT APPIMAGE_USER_TOOL_SETTINGS
)
