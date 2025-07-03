#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <atomic>

class KVStore;
class CommandHandler;

class TCPServer 
{
    public:

    /**
     * Constructor.
     * @param[in] context is the io context. 
     * @param[in] port is the port we want to run our server on.
    */
    TCPServer(boost::asio::io_context& context, int port);

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
};