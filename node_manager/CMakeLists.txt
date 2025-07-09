add_library(NodeManager "node_manager.h" "node_manager.cxx" "broadcast_queue.h" "broadcast_queue.cxx")
target_include_directories(NodeManager PUBLIC "./")
target_link_libraries(NodeManager PRIVATE 
    NodeParser 
    ${Boost_LIBRARIES}
    fmt
)