#include "node_manager.h"
#include "node_parser.h"
#include <fmt/format.h>
#include <iostream>


NodeManager::NodeManager(boost::asio::io_context& context, const std::vector<NodeInfo>& node_infos, int self_id) : m_context(context)
{
    for(size_t node_id = 0; node_id < node_infos.size(); node_id++) {
        if(node_id == self_id) {
            continue;
        }
        connectToNode(node_infos[node_id]);
    }

    m_broadcast_thread = std::thread([this](){
        broadcastLoop();
    });
}

void NodeManager::broadcastMessage(const std::string& message) 
{
    m_broadcast_queue.push(fmt::format("BROADCAST {}\n", message));
}

void NodeManager::connectToNode(const NodeInfo& node_info)
{
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_context);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(node_info.ip),node_info.port);
    socket->async_connect(endpoint, [this, socket, node_info](const boost::system::error_code& error_code){
        if(!error_code) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_connections.push_back(socket);
            std::cout << "Connected to peer " << node_info.ip << ":" << node_info.port << std::endl; 
        } else {
            std::cerr << "Failed to connect to peer " << node_info.ip << ":" << node_info.port << " " << error_code.message() << std::endl;
            auto timer = std::make_shared<boost::asio::steady_timer>(m_context, std::chrono::seconds(3));
            timer->async_wait([this, node_info, timer](const boost::system::error_code&) {
                connectToNode(node_info);
            });
        }
    });
}

void NodeManager::shutdown()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_shutdown = true;
    }
    m_broadcast_queue.shutdown();
    if(m_broadcast_thread.joinable()) {
        m_broadcast_thread.join();
    }
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connections.clear();
    }
}

void NodeManager::broadcastLoop()
{
    while(!m_shutdown) {
        auto message = m_broadcast_queue.pop();
        if (!message.has_value()) {
            continue;
        }
        sendToNodes(message.value());
    }
}

void NodeManager::sendToNodes(const std::string& message) 
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for(auto& socket: m_connections) {
        if(socket->is_open()) {
            boost::asio::async_write(*socket, boost::asio::buffer(message), [this, socket](const boost::system::error_code& error_code, size_t){
                /**
                 * We might wanna reconnect to the node here if it failed. 
                */
                if (error_code) {
                    std::cout << "Failed to send message, error: " << error_code.message() << std::endl;
                }
                
            });
        }
    }
}