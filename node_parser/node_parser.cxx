#include "node_parser.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

static constexpr char* CONFIG_FILE = "node_config.json";

NodeParser::NodeParser()
{
    std::fstream f(CONFIG_FILE);
    nlohmann::json data = nlohmann::json::parse(f);

    for(auto& info: data) {
        NodeInfo node_info;
        node_info.ip = info["ip"];
        node_info.port = info["port"];
        m_node_infos.push_back(node_info);
    }
}

void NodeParser::populateIDFromCMDArgs(int argc, char* argv[])
{
    for(int i=1; i<argc; i++) {
        std::string prefix = "--id=";
        std::string arg = argv[i];
        if(arg.rfind(prefix, 0) == 0) {
            auto id = arg.substr(prefix.size());
            try {
                m_id = stoi(id);
            } catch(...) {
                std::cerr << "Invalid value for --id" << std::endl;
            }
        }
    }
    if(!m_id.has_value()) {
        throw std::runtime_error("Missing --id=<node_id> argument");
    }    
}