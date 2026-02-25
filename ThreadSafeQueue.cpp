#include "ThreadSafeQueue.hpp"

void ThreadSafeQueue::push(Connection&& conn) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(std::move(conn));
    cond_.notify_one();
}

Connection ThreadSafeQueue::pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty())
        cond_.wait(lock);
    Connection conn = std::move(queue_.front());
    queue_.pop();
    return conn;
}

void ThreadSafeQueue::notify_all() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.notify_all();
}