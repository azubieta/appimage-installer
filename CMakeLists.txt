cmake_minimum_required(VERSION 3.8)

project(appimage-user-tool)

set(CMAKE_CXX_STANDARD 11)

# Versioning (mainly for packaging)
include(cmake/versioning.cmake)

# Use GNU Install dirs
include(GNUInstallDirs)


# Dependencies
# avoid collissions with existing libappimage target when used from other projects
if(NOT TARGET libappimage)
    find_package(libappimage REQUIRED)
endif()
find_package(Qt5 COMPONENTS Core Widgets Network XmlPatterns Gui REQUIRED)
find_package(XdgUtils COMPONENTS DesktopEntry REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(KF5Attica)

## Config
option(INSTALL_LIBAPPIMAGE "Install libAppImage as part of this project" OFF)
option(INSTALL_ATTICA "Install KF5 Attica as part of this project" OFF)

# Source
add_subdirectory(src)
add_subdirectory(res)

# Packaging
# only necessary when not being used from other projects
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    include(cmake/cpack_general.cmake)
    include(cmake/cpack_deb.cmake)
    include(cmake/cpack_rpm.cmake)
    include(CPack)
endif()
