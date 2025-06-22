#include "command_handler.h"
#include "kvstore.h"
#include<boost/tokenizer.hpp>
#include <vector>
#include <iostream>
#include <fmt/core.h>


CommandHandler::CommandHandler(const std::shared_ptr<KVStore>& store) : m_store(store) {}

std::string CommandHandler::handle(const std::string& command) 
{
    boost::char_separator<char> sep(" ");
    boost::tokenizer<boost::char_separator<char>> tok(command, sep);
    std::vector<std::string> tokens;
    for(boost::tokenizer<boost::char_separator<char>>::iterator it = tok.begin(); it != tok.end(); it++) {
        tokens.push_back(*it);
    }
    if (tokens.empty()) {
        return "Command cannot be empty.";
    }

    if (tokens[0] == "PUT") {
        if (tokens.size() != 3) {
            return "Wrong format for PUT command.";
        }
        auto result = fmt::format("Setting {} for {}", tokens[2], tokens[1]);
        m_store->set(tokens[1], tokens[2]);
        return result;
    } else if (tokens[0] == "GET") {
        if (tokens.size() != 2) {
            return "Wrong format for GET command.";
        }
        auto result = m_store->get(tokens[1]);
        if (result == std::nullopt) {
            return fmt::format("{} doesn't have a value stored.", tokens[1]);
        } else {
            return fmt::format("Value for key {} is {}", tokens[1], result.value());
        }
    } else if (tokens[0] == "DELETE") {
        if (tokens.size() != 2) {
            return "Wrong format for DELETE command.";
        }
        try {
            m_store->remove(tokens[1]);
        } catch(const std::runtime_error& ex) {
            return fmt::format("Error while deleting: {}", ex.what());
        }
    }
    return "Invalid command.";
}