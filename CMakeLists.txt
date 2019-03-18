cmake_minimum_required(VERSION 3.8)
project(appimage_user_tools VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)

# Use GNU Install dirs
include(GNUInstallDirs)

# Dependencies
find_package(libappimage REQUIRED)
find_package(Qt5 COMPONENTS Core Network XmlPatterns Gui REQUIRED)

# Source
add_subdirectory(src)
add_subdirectory(res)
