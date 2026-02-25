#include "ThreadPool.hpp"
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads, ThreadSafeQueue& q): queue_(q), running_(true) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers_.emplace_back([this] {
            while (running_) {
                Connection conn = queue_.pop();

                // conn.handle_request();
                //TEST
                std::cout << "[Worker] Handling fd=" << conn.get_fd() << std::endl;

                // Simulate processing time
                std::this_thread::sleep_for(std::chrono::seconds(5));

                conn.handle_request();  // optional: real request handling
                std::cout << "[Worker] Finished fd=" << conn.get_fd() << std::endl;
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    running_ = false;
    queue_.notify_all();
    for (auto& t : workers_)
        t.join();
}
