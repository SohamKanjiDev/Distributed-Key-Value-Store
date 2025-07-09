#pragma once
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include "broadcast_queue.h"

class NodeInfo;
class BroadcastQueue;

/**
 * Interface that takes care of broadcasting messages across the other nodes. 
*/
class NodeManager
{
public:

    /**
     * Constructor. 
     * @param[in] context is the io context. 
     * @param[in] node_infos: list containing the node info for all nodes. 
     * @param[in] self_id: is the id of the node of the current machine. 
    */
    NodeManager(boost::asio::io_context& context, const std::vector<NodeInfo>& node_infos, int self_id);

    /**
     * Broadcast the message across nodes. 
     * @param[in] message: is the message we want to broadcast. 
    */
    void broadcastMessage(const std::string& message);

    /**
     * API to start shuttind down the node manager. 
    */
    void shutdown();

private:

    /**
     * Connect to the node with the passed info. 
     * @param[in] node_info: is the info of the node we want to connect to. 
    */
    void connectToNode(const NodeInfo& node_info);

    /**
     * Runs in the background, popping messages out of the queue and broadcasting them. 
    */
    void broadcastLoop();

    /**
     * Send message to the nodes. 
     * @param[in] message: is the message to send.
    */
    void sendToNodes(const std::string& message);

    boost::asio::io_context& m_context;
    std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> m_connections;
    std::mutex m_mutex;
    BroadcastQueue m_broadcast_queue;
    std::thread m_broadcast_thread;
    bool m_shutdown = false;
};