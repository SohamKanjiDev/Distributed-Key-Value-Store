#include "command_handler.h"
#include "kvstore.h"
#include<boost/tokenizer.hpp>
#include <vector>
#include <iostream>
#include <fmt/core.h>
#include "response.h"
#include "node_manager.h"

CommandHandler::CommandHandler(const std::shared_ptr<KVStore>& store) : m_store(store) {}

std::string CommandHandler::handle(const std::string& command, const std::shared_ptr<NodeManager>& node_manager) 
{
    auto tokens = getCommandTokens(command);

    if (tokens.empty()) {
        return fmt::format("{}: Command cannot be empty", get_response_string(Response::INVALID_COMMAND));
    }

    auto is_broadcasted = isBroadcastedCommand(tokens[0]);

    std::vector<std::string> actual_tokens;
    // std::copy(tokens.begin() + is_broadcasted, tokens.end(), actual_tokens.begin());
    for(int i=is_broadcasted; i<tokens.size(); i++) {
        actual_tokens.push_back(tokens[i]);
    }

    auto broadcast_if_needed = [&]() {
        if(!is_broadcasted) {
            node_manager->broadcastMessage(command);
        }
    };

    if (actual_tokens[0] == "SET") {
        if (actual_tokens.size() != 3) {
            return  fmt::format("{}: Wrong format for SET command.", get_response_string(Response::INVALID_COMMAND));
        }
        auto result = fmt::format("{}: Setting {} for {}", get_response_string(Response::OK), actual_tokens[2], actual_tokens[1]);
        m_store->set(actual_tokens[1], actual_tokens[2]);
        broadcast_if_needed();
        return result;
    } else if (actual_tokens[0] == "GET") {
        if (actual_tokens.size() != 2) {
            return fmt::format("{}: Wrong format for GET command.", get_response_string(Response::INVALID_COMMAND));
        }
        auto result = m_store->get(actual_tokens[1]);
        if (result == std::nullopt) {
            return fmt::format("{}: {} doesn't have a value stored.", get_response_string(Response::KEY_NOT_FOUND), actual_tokens[1]);
        } else {
            return fmt::format("{}: Value for key {} is {}", get_response_string(Response::OK), actual_tokens[1], result.value());
        }
    } else if (actual_tokens[0] == "DELETE") {
        if (actual_tokens.size() != 2) {
            return fmt::format("{}: Wrong format for DELETE command.", get_response_string(Response::OK));
        }
        try {
            m_store->remove(actual_tokens[1]);
            broadcast_if_needed();
            return fmt::format("{}: {} removed.", get_response_string(Response::OK), actual_tokens[1]);
        } catch(const std::runtime_error& ex) {
            return fmt::format("{}: {}", get_response_string(Response::KEY_NOT_FOUND), ex.what());
        }
    }
    return "Invalid command.";
}

std::vector<std::string> CommandHandler::getCommandTokens(const std::string& command)
{
    boost::char_separator<char> sep(" ");
    boost::tokenizer<boost::char_separator<char>> tok(command, sep);
    std::vector<std::string> result;
    for(boost::tokenizer<boost::char_separator<char>>::iterator it = tok.begin(); it != tok.end(); it++) {
        result.push_back(*it);
    }
    return result;
}

bool CommandHandler::isBroadcastedCommand(const std::string& command_token)
{
    return command_token == "BROADCAST";
}