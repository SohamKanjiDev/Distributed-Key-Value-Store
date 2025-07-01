add_library(KVStore STATIC "kvstore.h" "kvstore.cxx") 
target_include_directories(KVStore PUBLIC "./")
target_link_libraries(KVStore PRIVATE sqlite3)