#pragma once
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <any>
#include <optional>


template<typename Task>
class SafeQueue
{
public:

    using BaseTaskReturnType = typename std::invoke_result_t<Task>;
    using PackagedTask = std::packaged_task<BaseTaskReturnType()>;

    using Iterator = typename std::list<PackagedTask>::iterator;

    SafeQueue()
    : m_head(m_tasks.begin())
    , m_tail(m_tasks.end())
    {
    }

    std::future<std::any> push(Task&& task)
    {
        std::lock_guard<std::mutex> lock(m_task_mtxs.second);

        PackagedTask packagedTask(std::move(task));
        std::future<BaseTaskReturnType> future(packagedTask.get_future());
        m_tasks.emplace_back(std::move(packagedTask));
        m_tail = m_tasks.end();
        m_size++;
        return future;
    }

    std::optional<PackagedTask> tryPop()
    {
        std::lock_guard<std::mutex> lock(m_task_mtxs.first);
        if (!isEmpty())
        {
            m_head++;
            m_tasks.erase(m_tasks.begin(), m_head);
            PackagedTask task = std::move(*m_head);
            m_size--;
            return std::optional<PackagedTask>{std::move(task)};
        }
        return std::nullopt;
    }

private:
    bool isEmpty()
    {
        return m_size == 0;
    }

private:
    std::atomic_size_t m_size{0};
    std::pair<std::mutex, std::mutex> m_task_mtxs; // .first -> head, .second -> tail
    std::list<PackagedTask> m_tasks;
    Iterator m_head, m_tail;
};