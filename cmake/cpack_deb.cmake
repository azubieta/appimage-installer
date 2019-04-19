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
set(CPACK_DEBIAN_PACKAGE_RELEASE "git${APPIMAGE_USER_TOOL_GIT_COMMIT_DATE_SHORT}.${APPIMAGE_USER_TOOL_GIT_COMMIT}")

if(CPACK_DEBIAN_COMPATIBILITY_LEVEL)
    set(CPACK_DEBIAN_PACKAGE_RELEASE "${CPACK_DEBIAN_PACKAGE_RELEASE}+${CPACK_DEBIAN_COMPATIBILITY_LEVEL}")
endif()

# Custom package names
set(CPACK_DEBIAN_APPIMAGE_USER_TOOL_PACKAGE_NAME appimage-user-tool)
set(CPACK_DEBIAN_APPIMAGE_USER_TOOL_SETTINGS_PACKAGE_NAME appimage-user-tool-settings)

# Dependencies
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
