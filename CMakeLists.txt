cmake_minimum_required(VERSION 3.8)

project(appimage_user_tools)

set(CMAKE_CXX_STANDARD 11)

# Versioning (mainly for packaging)
include(cmake/versioning.cmake)

# Use GNU Install dirs
include(GNUInstallDirs)

# Dependencies
find_package(libappimage REQUIRED)
find_package(Qt5 COMPONENTS Core Network XmlPatterns Gui REQUIRED)

# Source
add_subdirectory(src)
add_subdirectory(res)

# Packaging
include(cmake/cpack_general.cmake)
include(cmake/cpack_deb.cmake)
include(CPack)
