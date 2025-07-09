add_library(NodeParser STATIC "node_parser.h" "node_parser.cxx")
target_include_directories(NodeParser PUBLIC "./")
target_link_libraries(NodeParser PRIVATE
    nlohmann_json::nlohmann_json
)