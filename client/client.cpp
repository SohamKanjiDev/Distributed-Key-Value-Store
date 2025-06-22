#include <boost/asio.hpp>
#include <iostream>
#include <string>

constexpr static const char* INTRO = R"(Welcome!

Instructions:
1. SET <key> <value>
2. GET <key>
3. DELETE <key>
)";

constexpr int PORT = 12345;

namespace asio = boost::asio;
using asio::ip::tcp;

int main() {
    asio::io_context io;
    tcp::resolver resolver(io);
    auto endpoints = resolver.resolve("127.0.0.1", "12345");
    tcp::socket socket(io);
    boost::asio::connect(socket, endpoints);
    std::cout << INTRO;
    try {
        while(true) {
            std::cout << "Enter a command: \n";
            std::string command;
            std::getline(std::cin, command, '\n');
            command += "\n";
            asio::write(socket, asio::buffer(command));
            asio::streambuf buf;
            asio::read_until(socket, buf, "\n");
            std::istream is(&buf);
            std::string response;
            std::getline(is, response);
            std::cout << "Response: " << response << std::endl;
        }
    } catch(std::exception& ex) {
        std::cout << "Connection error: " << ex.what() << std::endl;
    }
}