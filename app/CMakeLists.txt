add_executable(KVStoreApp "main.cpp")
target_link_libraries(KVStoreApp PUBLIC
    TCPServer
)