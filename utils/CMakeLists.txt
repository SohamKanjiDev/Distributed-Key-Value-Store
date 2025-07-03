add_library(Utils STATIC "thread_pool.h" "thread_pool.cxx" "signal_manager.h" "signal_manager.cxx")
target_include_directories(Utils PUBLIC "./")