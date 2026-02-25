#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Connection.hpp"

class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    ThreadSafeQueue(ThreadSafeQueue&&) noexcept = default;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) noexcept = default;

    void push(Connection&& conn);
    Connection pop();
    void notify_all();
private:
    std::queue<Connection> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};
