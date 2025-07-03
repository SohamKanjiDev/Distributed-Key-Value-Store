#include "tcp_server.h"
#include <iostream>
#include "signal_manager.h"

int main() {
    SignalManager::setup();
    try {
        boost::asio::io_context io;
        TCPServer server(io, 12345);
        SignalManager::registerShutdownHandler([&](){
            server.shutdown();
        });
        server.run();
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
}