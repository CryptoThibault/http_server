#pragma once
#include "FileDescriptor.hpp"

class Connection {
public:
    explicit Connection(int client_fd);
    ~Connection() noexcept = default;

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&) noexcept = default;
    Connection& operator=(Connection&&) noexcept = default;

    void handle_request();
    int get_fd() const;
private:
    FileDescriptor fd_;
};

