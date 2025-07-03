#include "command_handler.h"
#include "kvstore.h"
#include<boost/tokenizer.hpp>
#include <vector>
#include <iostream>
#include <fmt/core.h>
#include "response.h"

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
        return fmt::format("{}: Command cannot be empty", get_response_string(Response::INVALID_COMMAND));
    }

    if (tokens[0] == "SET") {
        if (tokens.size() != 3) {
            return  fmt::format("{}: Wrong format for SET command.", get_response_string(Response::INVALID_COMMAND));
        }
        auto result = fmt::format("{}: Setting {} for {}", get_response_string(Response::OK), tokens[2], tokens[1]);
        m_store->set(tokens[1], tokens[2]);
        return result;
    } else if (tokens[0] == "GET") {
        if (tokens.size() != 2) {
            return fmt::format("{}: Wrong format for GET command.", get_response_string(Response::INVALID_COMMAND));
        }
        auto result = m_store->get(tokens[1]);
        if (result == std::nullopt) {
            return fmt::format("{}: {} doesn't have a value stored.", get_response_string(Response::KEY_NOT_FOUND), tokens[1]);
        } else {
            return fmt::format("{}: Value for key {} is {}", get_response_string(Response::OK), tokens[1], result.value());
        }
    } else if (tokens[0] == "DELETE") {
        if (tokens.size() != 2) {
            return fmt::format("{}: Wrong format for DELETE command.", get_response_string(Response::OK));
        }
        try {
            m_store->remove(tokens[1]);
            return fmt::format("{}: {} removed.", get_response_string(Response::OK), tokens[1]);
        } catch(const std::runtime_error& ex) {
            return fmt::format("{}: {}", get_response_string(Response::KEY_NOT_FOUND), ex.what());
        }
    }
    return "Invalid command.";
}