set(
    APP_SRCS
    main.cpp
    Command.h
    commands/SearchCommand.cpp
    commands/RemoveCommand.cpp
    commands/ListCommand.cpp
    commands/InstallCommand.cpp
    commands/UpdateCommand.cpp
)

add_executable(app ${APP_SRCS})

target_include_directories(app PUBLIC $<TARGET_PROPERTY:core,INTERFACE_INCLUDE_DIRECTORIES>)

target_link_libraries(
    app
    PRIVATE core
    PRIVATE Qt5::Core
    PRIVATE Qt5::Network
    PRIVATE KF5::Attica
    PRIVATE libappimage
    PRIVATE XdgUtils::DesktopEntry
    PRIVATE yaml-cpp
    PRIVATE AppImageUpdaterBridge
)

add_dependencies(app core)

# Install
install(
    TARGETS app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT APPIMAGE_INSTALLER
)
