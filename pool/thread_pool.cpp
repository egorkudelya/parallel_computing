#include "thread_pool.h"


ThreadPool::ThreadPool()
{
    size_t opt = std::thread::hardware_concurrency();
    for (int i = 0; i < opt - 1; i++)
    {
        m_threads.emplace_back(&ThreadPool::process, this);
    }
}

ThreadPool::ThreadPool(size_t poolSize)
{
    for (int i = 0; i < poolSize; i++)
    {
        m_threads.emplace_back(&ThreadPool::process, this);
    }
}

ThreadPool::~ThreadPool()
{
    shutDown();
}

void ThreadPool::process()
{
    while (!m_done)
    {
        auto optionalTask = m_queue.tryPop();
        if (optionalTask)
        {
            auto task = std::move(optionalTask.value());
            task();
        }
    }
}

void ThreadPool::shutDown()
{
    m_done = true;
    for (auto& m_thread: m_threads)
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
}
