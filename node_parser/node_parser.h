#pragma once
#include <string>
#include <vector>
#include <optional>

/**
 * Information about a particular node. 
*/
struct NodeInfo
{  
    std::string ip;
    int port;
};

/**
 * Parses node_config.json file and stores all node infos.
*/
class NodeParser
{
public:

    /**
     * Constructor. 
    */
    NodeParser();

    /**
     * Get all node infos. 
     * @returns node infos. 
    */
    std::vector<NodeInfo> getNodeInfos() {
        return m_node_infos;
    } 

    /**
     * @returns port for the current session. 
    */
    int getCurrentPort() {
        return m_node_infos[m_id.value()].port;
    }

    /**
     * @returns current node id.
    */
    int getCurrentNodeID() {
        return m_id.value();
    }

    /**
     * Return the current machine id from command line arguments.
    */
    void populateIDFromCMDArgs(int argc, char* argv[]);

private:
    std::vector<NodeInfo> m_node_infos;
    std::optional<int> m_id;
};