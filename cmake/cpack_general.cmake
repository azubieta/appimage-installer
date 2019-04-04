# general CPack options

set(PROJECT_VERSION ${APPIMAGEHUB_CLI_VERSION})
set(CPACK_GENERATOR "DEB")

# make sure to only package APPIMAGEHUB_CLI component
set(CPACK_COMPONENTS_ALL APPIMAGEHUB_CLI)
# package them all in a single package, otherwise cpack would generate one package per component by default
# https://cmake.org/cmake/help/v3.0/module/CPackComponent.html#variable:CPACK_COMPONENTS_GROUPING
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)

# global options
set(CPACK_PACKAGE_CONTACT "Alexis Lopez Zubieta")
set(CPACK_PACKAGE_HOMEPAGE "https://github.com/azubieta/appimage-user-tools")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

# versioning
# it appears setting CPACK_DEBIAN_PACKAGE_VERSION doesn't work, hence setting CPACK_PACKAGE_VERSION
set(CPACK_PACKAGE_VERSION ${APPIMAGEHUB_CLI_VERSION})

# TODO: insert some useful description
set(CPACK_COMPONENT_APPIMAGEHUB_CLI_PACKAGE_DESCRIPTION "AppImage User Tools")

# find more suitable section for package
set(CPACK_COMPONENT_APPIMAGEHUB_CLI_PACKAGE_SECTION misc)
