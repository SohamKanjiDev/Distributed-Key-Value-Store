#include "tcp_server.h"
#include <iostream>
#include "signal_manager.h"
#include "node_parser.h"
#include "node_manager.h"
#include <thread>
#include <memory>

int main(int argc, char* argv[]) {
    NodeParser parser;
    parser.populateIDFromCMDArgs(argc, argv);
    SignalManager::setup();
    boost::asio::io_context io;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard(io.get_executor());
    auto node_manager = std::make_shared<NodeManager>(io, parser.getNodeInfos(), parser.getCurrentNodeID());
    TCPServer server(io, node_manager, parser.getCurrentPort());
    SignalManager::registerShutdownHandler([&](){
        io.stop();
        server.shutdown();
        node_manager->shutdown();
    });
    std::thread io_thread([&]() {
        io.run();
    });
    server.run();
    work_guard.reset(); 
    io_thread.join();
}