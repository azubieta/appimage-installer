# required for DEB-DEFAULT to work as intended
cmake_minimum_required(VERSION 3.6)

# allow building Debian packages on non-Debian systems
if(DEFINED ENV{ARCH})
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE $ENV{ARCH})
    if(CPACK_DEBIAN_PACKAGE_ARCHITECTURE MATCHES "i686")
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
    elseif(CPACK_DEBIAN_PACKAGE_ARCHITECTURE MATCHES "x86_64")
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    endif()
endif()

# make sure to package components separately
#set(CPACK_DEB_PACKAGE_COMPONENT ON)
set(CPACK_DEB_COMPONENT_INSTALL ON)

# override default package naming
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

# Debian packaging global options
set(CPACK_DEBIAN_COMPRESSION_TYPE xz)

# use git hash as package release
set(CPACK_DEBIAN_PACKAGE_RELEASE "git${AIUT_GIT_COMMIT_DATE_SHORT}.${AIUT_GIT_COMMIT}")

# append build ID, similar to AppImage naming
if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(CPACK_DEBIAN_PACKAGE_RELEASE "travis$ENV{TRAVIS_BUILD_NUMBER}~${CPACK_DEBIAN_PACKAGE_RELEASE}")
else()
    set(CPACK_DEBIAN_PACKAGE_RELEASE "local~${CPACK_DEBIAN_PACKAGE_RELEASE}")
endif()

if(CPACK_DEBIAN_COMPATIBILITY_LEVEL)
    set(CPACK_DEBIAN_PACKAGE_RELEASE "${CPACK_DEBIAN_PACKAGE_RELEASE}+${CPACK_DEBIAN_COMPATIBILITY_LEVEL}")
endif()

# package name
# TODO: decide on package name
set(CPACK_DEBIAN_AIUT_PACKAGE_NAME "appimage-user-tools")

# TODO: review package list
set(CPACK_DEBIAN_AIUT_PACKAGE_DEPENDS "libqt5network5 (>= 5.2.1), libqt5core5a (>= 5.2.1), libappimage (>= 0.1.9, <= 0.2.0)")

# improve dependency list
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
