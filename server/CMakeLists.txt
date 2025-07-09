add_library(TCPServer STATIC "tcp_server.h" "tcp_server.cxx")
target_include_directories(TCPServer PUBLIC "./")

target_include_directories(TCPServer PUBLIC ${Boost_INCLUDE_DIRS} ${boost_asio_SOURCE_DIR}/include)
target_link_libraries(TCPServer PUBLIC 
    ${Boost_LIBRARIES}
    KVStore
    CommandHandler
    Utils
    NodeManager
)