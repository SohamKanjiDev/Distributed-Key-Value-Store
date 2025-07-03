#include <signal_manager.h>
#include <csignal>


std::vector<std::function<void()>> SignalManager::m_shutdown_handlers; 

void SignalManager::setup() {
    std::signal(SIGINT, handle);
}

void SignalManager::handle(int signum) {
    if(signum == SIGINT) {
        for(auto& handler: m_shutdown_handlers) {
            handler();
        }
    }
}

void SignalManager::registerShutdownHandler(const std::function<void()>& handler) {
    m_shutdown_handlers.push_back(handler);
}