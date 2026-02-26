#include "ThreadSafeQueue.hpp"

void ThreadSafeQueue::push(Connection&& conn) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(std::move(conn));
    cond_.notify_one();
}

Connection ThreadSafeQueue::pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty() && !stopped_)
        cond_.wait(lock);
    if (stopped_ && queue_.empty())
        return Connection(-1);
    Connection conn = std::move(queue_.front());
    queue_.pop();
    return conn;
}

void ThreadSafeQueue::notify_all() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.notify_all();
}

void ThreadSafeQueue::stop() {
    std::unique_lock<std::mutex> lock(mutex_);
    stopped_ = true;
    cond_.notify_all();
}