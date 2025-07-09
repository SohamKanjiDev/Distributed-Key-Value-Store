#include "broadcast_queue.h"

void BroadcastQueue::push(const std::string& message) 
{
    {
        std::lock_guard<std::mutex> lock(m_queue_mutex);
        if (m_shutdown) {
            return;
        }
        m_queue.push(message);
    }
    m_cv.notify_one();
}

std::optional<std::string> BroadcastQueue::pop()
{
    std::unique_lock<std::mutex> lock(m_queue_mutex);
    m_cv.wait(lock, [this]{return !m_queue.empty() || m_shutdown;});

    if (m_queue.empty() && m_shutdown) {
        return std::nullopt;
    }

    auto result = m_queue.front();
    m_queue.pop();
    return result;
}

bool BroadcastQueue::empty()
{
    std::lock_guard<std::mutex> lock(m_queue_mutex);
    return m_queue.empty();
}

void BroadcastQueue::shutdown()
{
    {
        std::lock_guard<std::mutex> lock(m_queue_mutex);
        m_shutdown = true;
    }
    m_cv.notify_all();
}