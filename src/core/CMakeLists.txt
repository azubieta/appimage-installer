add_library(
    core OBJECT
    Download.cpp
    FileDownload.cpp
    AppsLibrary.cpp
    Console.cpp
)

configure_module(core)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
