#include "Connection.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
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
        if (n == 0)
            break;
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
    std::string body = request.substr(header_end + 4);

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
    std::cout << "[INFO] Received GET " << path << std::endl;

    std::string clean = sanitize_path(path);
    if (clean.empty()) {
        send_response(400, "Bad Request\n");
        return;
    }

    std::string file_path = "./www" + clean;
    if (clean == "/")
        file_path = "./www/index.html";

    std::ifstream file(file_path);
    if (!file) {
        send_response(404, "Not Found\n");
        return;
    }

    std::string ext = get_extension(file_path);
    std::string content_type = "application/octet-stream";
    if (ext == ".html") content_type = "text/html";
    else if (ext == ".css") content_type = "text/css";
    else if (ext == ".js") content_type = "application/javascript";
    else if (ext == ".png") content_type = "image/png";
    else if (ext == ".jpg" || ext == ".jpeg") content_type = "image/jpeg";
    else if (ext == ".ico") content_type = "image/x-icon"; 
    
    std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    send_response(200, body, content_type);
}

void Connection::handle_post(const std::string& path, const std::string& body) {
    std::cout << "[INFO] Received POST " << path << std::endl;
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

void Connection::send_response(int status,  const std::string& body, const std::string& content_type) {
    std::ostringstream header;
    header << "HTTP/1.1 " << status << " OK\r\n"
           << "Content-Length: " << body.size() << "\r\n"
           << "Content-Type: " << content_type << "\r\n"
           << "\r\n";

    std::string header_str = header.str();
    ::write(fd_.get(), header_str.c_str(), header_str.size());
    ::write(fd_.get(), body.data(), body.size());
}

std::string Connection::sanitize_path(const std::string& raw) {
    if (raw.empty() || raw[0] != '/')
        return {};
    std::vector<std::string> parts;
    std::istringstream iss(raw);
    std::string tok;
    while (std::getline(iss, tok, '/')) {
        if (tok.empty() || tok == ".") continue;
        if (tok == "..") {
            if (!parts.empty()) parts.pop_back();
        } else {
            parts.push_back(tok);
        }
    }
    std::string clean = "/";
    for (size_t i = 0; i < parts.size(); ++i) {
        clean += parts[i];
        if (i + 1 < parts.size()) clean += "/";
    }
    return clean;
}

std::string Connection::get_extension(const std::string& path) {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    return path.substr(pos);
}

int Connection::get_fd() const {
    return fd_.get();
};