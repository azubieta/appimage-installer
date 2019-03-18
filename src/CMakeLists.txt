include(utils.cmake)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

add_subdirectory(commands)
add_subdirectory(entities)
add_subdirectory(gateways)

add_executable(
    app
    main.cpp
    Command.h
    $<TARGET_OBJECTS:commands>
    $<TARGET_OBJECTS:entities>
    $<TARGET_OBJECTS:gateways>
)

target_link_libraries(app
    PRIVATE Qt5::Core
    PRIVATE Qt5::Network
    PRIVATE Qt5::XmlPatterns
    PRIVATE Qt5::Gui
    PRIVATE libappimage
    )

target_include_directories(app PRIVATE ..)


# Install
install(
    TARGETS app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
