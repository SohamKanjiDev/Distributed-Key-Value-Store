add_library(CommandHandler STATIC "command_handler.h" "command_handler.cxx" "response.cxx")
target_include_directories(CommandHandler PUBLIC "./")
target_link_libraries(CommandHandler PUBLIC 
    KVStore
    fmt
    Utils
    ${Boost_LIBRARIES}
)