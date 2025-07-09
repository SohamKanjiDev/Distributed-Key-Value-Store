#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <atomic>
#include <string>

class KVStore;
class CommandHandler;
class NodeManager;

class TCPServer 
{
    public:

    /**
     * Constructor.
     * @param[in] context is the io context. 
     * @param[in] node_manager: is the node manager object. We will use this to broadcast messages to other nodes. 
     * @param[in] port is the port we want to run our server on.
    */
    TCPServer(boost::asio::io_context& context, const std::shared_ptr<NodeManager>& node_manager, int port);

    /**
     * Starts the server. 
    */
    void run();

    /**
     * Sets m_shutdown to true so that we can  
     * close server gracefully.
    */
    void shutdown();

    private: 

    /**
     * Handle the client.
     * 
     * @param[in] socket is the shared pointer to socket. 
     * @param[in] handler is the command handler.
     * 
    */
    void handleClient(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket, CommandHandler& handler);

    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::io_context& m_io_context;
    std::shared_ptr<KVStore> m_store;
    std::atomic<bool> m_shutdown{false};
    std::string m_db_file_name;
    std::shared_ptr<NodeManager> m_node_manager;
};