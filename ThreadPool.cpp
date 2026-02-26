#include "ThreadPool.hpp"
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads, ThreadSafeQueue& q): queue_(q) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers_.emplace_back([this] {
            while (true) {
                Connection conn = queue_.pop();
                    if (conn.get_fd() < 0)
                        break;
                conn.handle_request();

                // //[TEST] Processing time for handle several connexions
                // std::cout << "[Worker] Handling fd=" << conn.get_fd() << std::endl;
                // std::this_thread::sleep_for(std::chrono::seconds(5));
                // conn.handle_request();
                // std::cout << "[Worker] Finished fd=" << conn.get_fd() << std::endl;
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    queue_.stop();
    for (auto& t : workers_)
        t.join();
}