#pragma once
#include <vector>
#include <thread>
#include "ThreadSafeQueue.hpp"

class ThreadPool {
public:
    ThreadPool(size_t numThreads, ThreadSafeQueue& q);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
private:
    std::vector<std::thread> workers_;
    ThreadSafeQueue& queue_;
};
