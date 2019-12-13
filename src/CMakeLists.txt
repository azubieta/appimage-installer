# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

if(INSTALL_LIBAPPIMAGE)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
endif()

add_subdirectory(core)
add_subdirectory(cli)

# Install libappimage with the main executable
if(INSTALL_LIBAPPIMAGE)
    install(
        FILES
        $<TARGET_SONAME_FILE:libappimage>
        $<TARGET_LINKER_FILE:libappimage>
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME} COMPONENT APPIMAGE_INSTALLER
    )
endif()

if(INSTALL_ATTICA)
    install(
        FILES
        $<TARGET_SONAME_FILE:KF5::Attica>
        $<TARGET_LINKER_FILE:KF5::Attica>
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME} COMPONENT APPIMAGE_INSTALLER
    )
endif()
