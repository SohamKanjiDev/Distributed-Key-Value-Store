#pragma once
#include <string> 


enum class Response 
{
    OK,
    KEY_NOT_FOUND,
    INVALID_COMMAND
};

/**
 * @returns the response string for the response enum.
*/
std::string get_response_string(Response response);