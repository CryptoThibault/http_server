#pragma once
#include "FileDescriptor.hpp"
#include <iostream>

class Connection {
public:
    explicit Connection(int fd);
    ~Connection() noexcept = default;

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&) noexcept = default;
    Connection& operator=(Connection&&) noexcept = default;

    void handle_request();
    int get_fd() const;
private:
    FileDescriptor fd_;

    void handle_get(const std::string& path);
    void handle_post(const std::string& path, const std::string& body);
    void send_response(int status_code, const std::string& body, const std::string& content_type = "text/plain");
};

