add_library(
    core STATIC
    Download.cpp
    FileDownload.cpp
    AppsLibrary.cpp
    Console.cpp
    Settings.cpp
)

configure_module(core)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core PRIVATE yaml-cpp)