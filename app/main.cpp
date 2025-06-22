#include "tcp_server.h"
#include <iostream>

int main() {
    try {
        boost::asio::io_context io;
        TCPServer server(io, 12345);
        server.run();
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
}