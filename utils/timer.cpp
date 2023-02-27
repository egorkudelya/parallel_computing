#include "timer.h"

void Timer::start(const std::string& message)
{
    auto now = std::chrono::steady_clock::now();
    m_data[message] = now;
}

void Timer::commit(const std::string& message)
{
    auto start = m_data.find(message);
    if (start == m_data.end())
    {
        std::cerr << "Unknown commit message" << std::endl;
        return;
    }
    auto now = std::chrono::steady_clock::now();
    m_data[message] = std::chrono::time_point<std::chrono::steady_clock>(now - start->second);
}

void Timer::print()
{
    for (const auto& commit: m_data)
    {
        std::cout << commit.first << " --> " << commit.second.time_since_epoch().count()/1e6 << " ms"  << std::endl;
    }
}

