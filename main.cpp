#include "Listener.hpp"
#include "Connection.hpp"
#include "ThreadSafeQueue.hpp"
#include "ThreadPool.hpp"

#include <iostream>
#include <cstdlib>
#include <csignal>

std::atomic<bool> running(true);
std::atomic<int> port(8080);

void signal_handler(int) {
    std::cout << "\n[INFO] Signal received, pressing Ctrl+C to confirm." << std::endl;
    running = false;
    std::string cmd = "curl -s http://127.0.0.1:" + std::to_string(port) + " > /dev/null";
    system(cmd.c_str());
}

int main(int argc, char* argv[]) {
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
            if (!running) break;
            Connection conn(client_fd);
            queue.push(std::move(conn));
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n[INFO] Shutdown complete." << std::endl;
    return 0;
}