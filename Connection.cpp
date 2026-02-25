#include "Connection.hpp"
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>

Connection::Connection(int client_fd) : fd_(client_fd) {}

void Connection::handle_request() {
    char buffer[1024];
    ssize_t bytes = ::read(fd_.get(), buffer, sizeof(buffer) - 1);
    if (bytes <= 0) {
        std::cerr << "Read failed or client closed" << std::endl;
        return;
    }

    buffer[bytes] = '\0';
    std::string request(buffer);

    if (request.rfind("GET ", 0) == 0) {
        const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 13\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Hello world!\n";
        ::write(fd_.get(), response, std::strlen(response));
    } else {
        const char* response =
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Length: 11\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Bad Request";
        ::write(fd_.get(), response, std::strlen(response));
    }
}