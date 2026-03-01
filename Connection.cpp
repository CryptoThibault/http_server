#include "Connection.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <algorithm>

#include <atomic>

extern std::atomic<bool> running;

Connection::Connection(int fd) : fd_(fd) {}

void Connection::handle_request() {
    std::string request;
    char buf[1024];
    ssize_t n;

    struct timeval tv{1,0};
    setsockopt(fd_.get(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (true) {
        n = read(fd_.get(), buf, sizeof(buf));
        if (n > 0) {
            request.append(buf, n);
            if (request.find("\r\n\r\n") != std::string::npos)
                break;
            continue;
        }

        if (n == 0) {
            break;
        }

        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            if (!running)
                return;
            else
                continue;
        }

        std::cerr << "[ERROR] read failed: " << strerror(errno) << "\n";
        return;
    }

    if (request.empty())
        return;

    size_t header_end = request.find("\r\n\r\n");
    if (header_end == std::string::npos)
        return;
    std::string headers = request.substr(0, header_end);
    std::string body = request.substr(header_end + 4); // initial body data

    size_t content_length = 0;
    std::istringstream header_stream(headers);
    std::string line;
    while (std::getline(header_stream, line)) {
        if (line.find("Content-Length:") == 0) {
            content_length = std::stoul(line.substr(15));
        }
    }

    while (body.size() < content_length) {
        char temp[1024];
        ssize_t m = read(fd_.get(), temp, sizeof(temp));
        if (m <= 0) break;
        body.append(temp, m);
    }

    std::istringstream first_line(headers);
    std::string method, path;
    first_line >> method >> path;

    if (method == "GET") handle_get(path);
    else if (method == "POST") handle_post(path, body);
    else send_response(405, "Method Not Allowed");
}

void Connection::handle_get(const std::string& path) {
    if (path == "/") {
        send_response(200, "Hello, World!");
    } else if (path == "/hi") {
        send_response(200, "Hi there!");
    } else if (path == "/about") {
        send_response(200, "This is a demo server");
    } else if (path == "/html") {
        send_response(200, "<!DOCTYPE html><html><body><h1>HTML Response</h1><p>This is an HTML response.</p></body></html>", "text/html");
    } else if (path == "/c++") {
        send_response(200, "#include <iostream>\n\nint main() {\n    std::cout << \"Hello from C++\" << std::endl;\n    return 0;\n}", "text/plain");
    } else {
        send_response(404, "Not Found");
    }
}

void Connection::handle_post(const std::string& path, const std::string& body) {
    std::cout << "[INFO] Received POST request" << std::endl;
    std::cout << "[INFO] Path: " << path << std::endl;
    std::cout << "[INFO] Body:\n" << body << std::endl;

    if (path == "/") {
        send_response(200, "Data received: " + body);
    } else if (path == "/echo") {
        send_response(200, body);
    } else if (path == "/reverse") {
        std::string reversed = body;
        std::reverse(reversed.begin(), reversed.end());
        send_response(200, reversed);
    } else {
        send_response(404, "POST route not found");
    }
}

void Connection::send_response(int status_code, const std::string& body, const std::string& content_type) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " "
             << (status_code == 200 ? "OK" : (status_code == 404 ? "Not Found" : "Error")) 
             << "\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body << "\n";

    const std::string resp_str = response.str();
    ssize_t bytes_written = write(fd_.get(), resp_str.c_str(), resp_str.size());
    if (bytes_written < 0) {
        std::cerr << "[ERROR] Failed to write to client: " << strerror(errno) << std::endl;
    }
}

int Connection::get_fd() const {
    return fd_.get();
};