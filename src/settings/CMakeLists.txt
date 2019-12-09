# Common settings module
add_library(settings-common OBJECT Settings.cpp Settings.h)

configure_module(settings-common)
target_include_directories(settings-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(
    appimage-installer-settings
    main.cpp
    SettingsDialog.h
    SettingsDialog.cpp
    SettingsDialog.ui

    $<TARGET_OBJECTS:settings-common>
)

configure_module(appimage-installer-settings)
target_link_libraries(appimage-installer-settings PRIVATE Qt5::Core Qt5::Widgets yaml-cpp)

add_dependencies(appimage-installer-settings settings-common)

# Install
install(
    TARGETS appimage-installer-settings
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT APPIMAGE_INSTALLER
)

