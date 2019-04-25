##################################
# borrowed from AppImageLauncher #
##################################

set(APPIMAGE_USER_TOOL_V_MAJOR 0)
set(APPIMAGE_USER_TOOL_V_MINOR 1)
set(APPIMAGE_USER_TOOL_V_PATCH 3)

set(APPIMAGE_USER_TOOL_VERSION ${APPIMAGE_USER_TOOL_V_MAJOR}.${APPIMAGE_USER_TOOL_V_MINOR}.${APPIMAGE_USER_TOOL_V_PATCH}${APPIMAGE_USER_TOOL_V_SUFFIX})

# check whether git is available
find_program(GIT git)
set(GIT_COMMIT_CACHE_FILE "${PROJECT_SOURCE_DIR}/cmake/GIT_COMMIT")

if(NOT GIT STREQUAL "GIT-NOTFOUND")
    # read Git revision ID
    # WARNING: this value will be stored in the CMake cache
    # to update it, you will have to reset the CMake cache
    # (doesn't matter for CI builds like Travis for instance, where there's no permanent CMake cache)
    execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE APPIMAGE_USER_TOOL_GIT_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        RESULT_VARIABLE GIT_RESULT
    )

    if(GIT_RESULT EQUAL 0)
        message(STATUS "Storing git commit ID in cache file")
        file(WRITE "${GIT_COMMIT_CACHE_FILE}" "${APPIMAGE_USER_TOOL_GIT_COMMIT}")
    endif()
endif()

if(NOT GIT_RESULT EQUAL 0)
    # git call failed or git hasn't been found, might happen when calling CMake in an extracted source tarball
    # therefore we try to find the git commit cache file
    # if it doesn't exist, refuse to configure

    message(WARNING "Failed to gather commit ID via git command, trying to read cache file")
    if(EXISTS "${GIT_COMMIT_CACHE_FILE}")
        file(READ "${GIT_COMMIT_CACHE_FILE}" APPIMAGE_USER_TOOL_CACHED_GIT_COMMIT)
        mark_as_advanced(FORCE APPIMAGE_USER_TOOL_CACHED_GIT_COMMIT)
        string(REPLACE "\n" "" APPIMAGE_USER_TOOL_GIT_COMMIT "${APPIMAGE_USER_TOOL_CACHED_GIT_COMMIT}")
    else()
        message(FATAL_ERROR "Could not find git commit cache file, git commit ID not available for versioning")
    endif()
endif()

if("${APPIMAGE_USER_TOOL_GIT_COMMIT}" STREQUAL "")
    message(FATAL_ERROR "Invalid git commit ID: ${APPIMAGE_USER_TOOL_GIT_COMMIT}")
endif()

message(STATUS "Git commit: ${APPIMAGE_USER_TOOL_GIT_COMMIT}")
mark_as_advanced(FORCE APPIMAGE_USER_TOOL_GIT_COMMIT)

# add build number based on Travis build number if possible
if("$ENV{TRAVIS_BUILD_NUMBER}" STREQUAL "")
    set(APPIMAGE_USER_TOOL_BUILD_NUMBER "<local dev build>")
else()
    set(APPIMAGE_USER_TOOL_BUILD_NUMBER "$ENV{TRAVIS_BUILD_NUMBER}")
endif()

# get current date
execute_process(
    COMMAND env LC_ALL=C date -u "+%Y-%m-%d %H:%M:%S %Z"
    OUTPUT_VARIABLE APPIMAGE_USER_TOOL_BUILD_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# get current date, short form
execute_process(
    COMMAND env LC_ALL=C git show -s --format=%ci ${APPIMAGE_USER_TOOL_GIT_COMMIT}
    OUTPUT_VARIABLE APPIMAGE_USER_TOOL_GIT_COMMIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
execute_process(
    COMMAND env LC_ALL=C date -u "+%Y%m%d" -d ${APPIMAGE_USER_TOOL_GIT_COMMIT_DATE}
    OUTPUT_VARIABLE APPIMAGE_USER_TOOL_GIT_COMMIT_DATE_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_definitions(-DAPPIMAGE_USER_TOOL_VERSION="${APPIMAGE_USER_TOOL_VERSION}")
add_definitions(-DAPPIMAGE_USER_TOOL_GIT_COMMIT="${APPIMAGE_USER_TOOL_GIT_COMMIT}")
add_definitions(-DAPPIMAGE_USER_TOOL_BUILD_NUMBER="${APPIMAGE_USER_TOOL_BUILD_NUMBER}")
add_definitions(-DAPPIMAGE_USER_TOOL_BUILD_DATE="${APPIMAGE_USER_TOOL_BUILD_DATE}")

