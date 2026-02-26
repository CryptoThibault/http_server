#include "Listener.hpp"
#include "Connection.hpp"
#include "ThreadSafeQueue.hpp"
#include "ThreadPool.hpp"

#include <iostream>
#include <cstdlib>
#include <csignal>

std::atomic<bool> running(true);

void signal_handler(int) {
    std::cout << "\n[INFO] Signal received, shutting down..." << std::endl;
    running = false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        std::cerr << "Invalid port number: " << port << std::endl;
        return 1;
    }

    std::signal(SIGINT, signal_handler);
    
    try {
        Listener listener(static_cast<uint16_t>(port));
        std::cout << "[INFO] Server listening on port " << port << std::endl;

        ThreadSafeQueue queue;
        size_t numThreads = 4;
        ThreadPool pool(numThreads, queue);

        while (running) {
            int client_fd = listener.accept();
            Connection conn(client_fd);
            queue.push(std::move(conn));
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}