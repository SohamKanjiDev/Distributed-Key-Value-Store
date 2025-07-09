#pragma once

#include <thread>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <vector>
#include <optional>

/**
 * Broadcast queue class which is used for sending out messages. 
*/
class BroadcastQueue
{
public:

    /**
     * Pushes the message into the queue. 
     * @param[in] message: is the message we want to push. 
    */
    void push(const std::string& message);

    /**
     * Pops the message out from queue.
     * @returns the popped out message 
    */
    std::optional<std::string> pop();
    
    /**
     * Checks if the queue is empty. 
     * @returns true if the queue is empty. 
    */
    bool empty();

    /**
     * Shuts down the queue.
    */
    void shutdown();

private:

    std::queue<std::string> m_queue;
    std::mutex m_queue_mutex;
    std::condition_variable m_cv;
    bool m_shutdown = false;
};
