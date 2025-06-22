#pragma once
#include <string>
#include <memory>

class KVStore;

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
     * @returns the response.
    */ 
    std::string handle(const std::string& command);

    private:

    std::shared_ptr<KVStore> m_store;
};