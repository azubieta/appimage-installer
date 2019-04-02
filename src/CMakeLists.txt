include(utils.cmake)

find_package(KF5Attica)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

if(INSTALL_LIBAPPIMAGE)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/appmage-user-tools")
endif()

add_executable(
    app
    main.cpp
    Command.h
    entities/Application.cpp
    gateways/FileDownload.cpp
    gateways/Download.cpp
    commands/SearchCommand.cpp
    commands/RemoveCommand.cpp
    commands/ListCommand.cpp
    commands/InstallCommand.cpp
)

target_link_libraries(
    app
    PRIVATE Qt5::Core
    PRIVATE Qt5::Network
    PRIVATE KF5::Attica
    PRIVATE libappimage
)

# Install
install(
    TARGETS app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT AIUT
)

# Install libappimage with the main executable
if(INSTALL_LIBAPPIMAGE)
    install(
        FILES
        $<TARGET_SONAME_FILE:libappimage>
        $<TARGET_LINKER_FILE:libappimage>
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/appmage-user-tools COMPONENT AIUT
    )
endif()

if(INSTALL_ATTICA)
    install(
        FILES
        $<TARGET_SONAME_FILE:KF5::Attica>
        $<TARGET_LINKER_FILE:KF5::Attica>
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/appmage-user-tools COMPONENT AIUT
    )
endif()
