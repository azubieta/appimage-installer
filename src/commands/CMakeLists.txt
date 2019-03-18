add_library(
    commands OBJECT
    GetCommand.cpp
    ListCommand.cpp
    RemoveCommand.cpp
    SearchCommand.cpp
)

configure_module(commands)
