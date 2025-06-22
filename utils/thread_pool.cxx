#include "thread_pool.h"

ThreadPool::ThreadPool(size_t num_threads) {
    for(auto i = 0; i < num_threads; i++) {
        m_worker_threads.emplace_back([this]{
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock lock(m_queue_mutex);
                    m_cv.wait(lock, [this]{return m_stop || !m_tasks_queue.empty(); });

                    if(m_stop && m_tasks_queue.empty()) {
                        return;
                    }

                    task = m_tasks_queue.front();
                    m_tasks_queue.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock lock(m_queue_mutex);
        m_stop = true;
    }

    m_cv.notify_all();

    for(auto& worker: m_worker_threads) {
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock lock(m_queue_mutex);
        m_tasks_queue.emplace(std::move(task));
    }

    m_cv.notify_one();
}
