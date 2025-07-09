#include "tcp_server.h"
#include <iostream>
#include <fstream>
#include "kvstore.h"
#include "command_handler.h"
#include "thread_pool.h"
#include <thread>
#include <chrono>
#include "node_manager.h"

namespace asio = boost::asio;
using asio::ip::tcp;

TCPServer::TCPServer(asio::io_context& context, const std::shared_ptr<NodeManager>& node_manager, int port) : m_io_context(context),m_acceptor(tcp::acceptor(context, tcp::endpoint(tcp::v4(), port))) {
    m_db_file_name = "backup_"+std::to_string(port)+".db";
    m_store = std::make_shared<KVStore>(m_db_file_name);
    m_node_manager = node_manager;
}

void TCPServer::handleClient(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket, CommandHandler& handler)
{
    try {
        while(true) {
            asio::streambuf buf;
            asio::read_until(*socket, buf, "\n");
            std::istream is(&buf);
            std::string command;
            std::getline(is, command);
            auto response = handler.handle(command, m_node_manager) + "\n";
            asio::write(*socket, asio::buffer(response));
        }
    } catch(const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
}

void TCPServer::shutdown() {
    m_shutdown = true;
    m_acceptor.close();
}

void TCPServer::run() {
    std::thread background_thread([this](){
        while(!m_shutdown.load()) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            m_store->updateDB(m_db_file_name);
        }
        m_store->updateDB(m_db_file_name);
    });
    CommandHandler command_handler(m_store);
    ThreadPool thread_pool;
    while (!m_shutdown.load()) {
        try {
            auto socket = std::make_shared<tcp::socket>(m_io_context);
            m_acceptor.accept(*socket);
            thread_pool.enqueue([socket, &command_handler, this]() mutable {
                handleClient(socket, command_handler);
            });
            
        } catch(const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
    background_thread.join();
}