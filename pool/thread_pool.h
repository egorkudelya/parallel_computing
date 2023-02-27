#pragma once
#include "../queue/queue.h"
#include <any>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    ThreadPool();

    explicit ThreadPool(size_t poolSize);

    ~ThreadPool();
    void shutDown();

    template <typename Func>
    std::future<std::any> pushTask(Func&& clientTask)
    {
        static_assert(std::is_invocable_v<Func> && "pushTask accepts only callable objects");
        return m_queue.push(std::forward<Func>(clientTask));
    }

    void process();

private:
    SafeQueue<std::function<std::any()>> m_queue;
    std::atomic<bool> m_done{false};
    std::vector<std::thread> m_threads;
};