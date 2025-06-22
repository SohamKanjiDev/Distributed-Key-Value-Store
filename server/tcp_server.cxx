#include "tcp_server.h"
#include <iostream>
#include <string>
#include <fstream>
#include "kvstore.h"
#include "command_handler.h"
#include "thread_pool.h"

namespace asio = boost::asio;
using asio::ip::tcp;

TCPServer::TCPServer(asio::io_context& context, int port) : m_io_context(context),m_acceptor(tcp::acceptor(context, tcp::endpoint(tcp::v4(), port))) {
    m_store = std::make_shared<KVStore>();
}

void TCPServer::handleClient(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket, CommandHandler& handler)
{
    while(true) {
        asio::streambuf buf;
        asio::read_until(*socket, buf, "\n");
        std::istream is(&buf);
        std::string command;
        std::getline(is, command);
        auto response = handler.handle(command) + "\n";
        asio::write(*socket, asio::buffer(response));
    }
}

void TCPServer::run() {
    CommandHandler command_handler(m_store);
    ThreadPool thread_pool;
    while (true) {
        try {
            std::cout << "Waiting for client!" << std::endl;
            auto socket = std::make_shared<tcp::socket>(m_io_context);
            m_acceptor.accept(*socket);
            std::cout << "Client Connected!!" << std::endl;
            thread_pool.enqueue([socket, &command_handler, this]() mutable {
                handleClient(socket, command_handler);
            });
            
        } catch(const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
    
    
}