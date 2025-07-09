#pragma once
#include <string>
#include <memory>
#include <vector>

class KVStore;
class NodeManager;

class CommandHandler
{
    public:

    /**
     * Constructor. 
     * 
     * @param[in] store is shared pointer to the store object we are handling. 
    */
    CommandHandler(const std::shared_ptr<KVStore>& store);

    /**
     * Sends back the response given the command. 
     * 
     * @param[in] command is the command we are sending.
     * @param[in] node_manager is the node manager we will use to broadcast SET/DELETE commands to other nodes. 
     * @returns the response.
    */ 
    std::string handle(const std::string& command, const std::shared_ptr<NodeManager>& node_manager);

    private:
    
    /**
     * Checks if the command received is broadcasted using the first command token.
    */
    bool isBroadcastedCommand(const std::string& command_token);

    /**
     * @returns the command tokens for the passed command.
    */
    std::vector<std::string> getCommandTokens(const std::string& command);

    std::shared_ptr<KVStore> m_store;
};