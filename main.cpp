#include "Listener.hpp"
#include "Connection.hpp"
#include "ThreadSafeQueue.hpp"
#include "ThreadPool.hpp"

#include <iostream>
#include <cstdlib>
#include <csignal>
#include <atomic>

std::atomic<bool> running(true);
int g_listen_fd = -1;

void signal_handler(int) {
    std::cout << "\n[INFO] Signal received, shutting down..." << std::endl;
    running = false;
    if (g_listen_fd >= 0) {
        ::close(g_listen_fd);
        g_listen_fd = -1;
    }
}

int main(int argc, char* argv[]) {
    int port = 8080;

    if (argc != 1 && argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
        return 1;
    }

    if (argc == 2)
        port = std::atoi(argv[1]);

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
            if (client_fd < 0) break;
            queue.push(Connection(client_fd));
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n[INFO] Shutdown complete." << std::endl;
    return 0;
}