add_executable(
    app
    main.cpp
    Command.h
    commands/SearchCommand.cpp
    commands/RemoveCommand.cpp
    commands/ListCommand.cpp
    commands/InstallCommand.cpp

    $<TARGET_OBJECTS:settings-common>
    $<TARGET_OBJECTS:core>
)

configure_module(app)

target_include_directories(app PUBLIC $<TARGET_PROPERTY:core,INTERFACE_INCLUDE_DIRECTORIES>)
target_include_directories(app PUBLIC $<TARGET_PROPERTY:settings-common,INTERFACE_INCLUDE_DIRECTORIES>)

target_link_libraries(
    app
    PRIVATE Qt5::Core
    PRIVATE Qt5::Network
    PRIVATE KF5::Attica
    PRIVATE libappimage
    PRIVATE XdgUtils::DesktopEntry
    PRIVATE yaml-cpp
)

add_dependencies(app core settings-common)

# Install
install(
    TARGETS app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT APPIMAGE_USER_TOOL
)
