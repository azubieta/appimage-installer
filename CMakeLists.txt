cmake_minimum_required(VERSION 3.8)

project(appimagehub_cli)

set(CMAKE_CXX_STANDARD 11)

# Versioning (mainly for packaging)
include(cmake/versioning.cmake)

# Use GNU Install dirs
include(GNUInstallDirs)

# Dependencies
find_package(libappimage REQUIRED)
find_package(Qt5 COMPONENTS Core Network XmlPatterns Gui REQUIRED)

## Config
option(INSTALL_LIBAPPIMAGE "Install libAppImage as part of this project" OFF)
option(INSTALL_ATTICA "Install KF5 Attica as part of this project" OFF)


# Source
add_subdirectory(src)
add_subdirectory(res)

# Packaging
include(cmake/cpack_general.cmake)
include(cmake/cpack_deb.cmake)
include(cmake/cpack_rpm.cmake)
include(CPack)
