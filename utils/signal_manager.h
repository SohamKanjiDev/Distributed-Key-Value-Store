#pragma once
#include <functional>
#include <vector>

/**
 * Handles the execution of callbacks for csignals. 
*/
class SignalManager 
{

public:
    /**
     * Sets up the connection to signal. 
    */
    static void setup();

    /**
     * Registers the passed handler so that it can be called 
     * when receive the SIGINT signal.
     * @param[in] handler is the handler.
    */
    static void registerShutdownHandler(const std::function<void()>& handler);

private:

    /**
     * Responsible for calling the appropriate handlers when a particuar signal is received. 
     * @param[in] signum: is the signal identifier. 
    */
    static void handle(int signum);

    static std::vector<std::function<void()>>  m_shutdown_handlers;

};