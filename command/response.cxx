#include "response.h"
#include <unordered_map>

static const std::unordered_map<Response, std::string> RESPONSE_ENUM_TO_STRING = {
    {Response::OK, "OK"},
    {Response::KEY_NOT_FOUND, "KEY NOT FOUND"},
    {Response::INVALID_COMMAND, "INVALID COMMAND"}
};

std::string get_response_string(Response response)
{
    return RESPONSE_ENUM_TO_STRING.at(response);
}