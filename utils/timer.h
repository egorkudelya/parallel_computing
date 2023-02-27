#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>

class Timer
{
public:
    Timer() = default;
    void start(const std::string& message);
    void commit(const std::string& message);
    void print();

private:
    std::unordered_map<std::string, std::chrono::time_point<std::chrono::steady_clock>> m_data;
};

