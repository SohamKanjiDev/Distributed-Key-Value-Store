#pragma once
#include <thread>
#include <functional>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <vector>

/**
 * Implements thread pool. 
*/
class ThreadPool 
{

    public: 
    /**
     * Constructor for the thread pool. 
     * Initialises the worker threads. 
     * 
     * @param[in] num_threads is the number of threads we want.
    */
    ThreadPool(size_t num_threads = std::thread::hardware_concurrency());

    /**
     * Destructor for the thread pool.
    */
    ~ThreadPool();
    
    /**
     * Adds the given task to the tasks queue. 
     * 
     * @param[in] task is the given task.
    */
    void enqueue(std::function<void()> task);

    private:

    std::condition_variable m_cv;
    std::queue<std::function<void()>> m_tasks_queue;
    std::vector<std::thread> m_worker_threads;
    std::mutex m_queue_mutex;
    bool m_stop;

};