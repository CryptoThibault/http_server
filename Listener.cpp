#include "Listener.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cerrno>

extern int g_listen_fd;

Listener::Listener(uint16_t port) : port_(port) {
    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd_.get() < 0)
        throw std::runtime_error("Failed to create socket");
    g_listen_fd = fd_.get();

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    int opt = 1;
    if (setsockopt(fd_.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt SO_REUSEADDR failed");

    if (::bind(fd_.get(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        throw std::runtime_error("Bind failed");

    if (::listen(fd_.get(), SOMAXCONN) < 0)
        throw std::runtime_error("Listen failed");
}

int Listener::accept() {
    int client_fd = ::accept(fd_.get(), nullptr, nullptr);
    if (client_fd < 0) {
        if (errno == EBADF || errno == EINVAL)
            return -1;
        throw std::runtime_error("Accept failed");
    }
    return client_fd;
}