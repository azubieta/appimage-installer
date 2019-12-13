cmake_minimum_required(VERSION 3.8)

project(appimage-installer VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 11)

# Versioning (mainly for packaging)
include(cmake/versioning.cmake)

# Use GNU Install dirs
include(GNUInstallDirs)


## Config
option(USE_CONAN "Use conan as source of dependencies" OFF)
option(INSTALL_LIBAPPIMAGE "Install libAppImage as part of this project" OFF)
option(INSTALL_ATTICA "Install KF5 Attica as part of this project" OFF)

# Dependencies
if (USE_CONAN)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(NO_OUTPUT_DIRS)
endif ()

# avoid collissions with existing libappimage target when used from other projects
if (NOT TARGET libappimage)
    find_package(libappimage REQUIRED)
endif ()

find_package(Qt5 COMPONENTS Core Widgets Network Gui REQUIRED)
find_package(XdgUtils COMPONENTS DesktopEntry REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(KF5Attica REQUIRED)
find_package(AppImageUpdaterBridge REQUIRED)

# Source
add_subdirectory(src)
add_subdirectory(res)

# Packaging
# only necessary when not being used from other projects
if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    include(cmake/cpack_general.cmake)
    include(cmake/cpack_deb.cmake)
    include(cmake/cpack_rpm.cmake)
    include(CPack)
endif ()
