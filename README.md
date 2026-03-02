# C++20 HTTP Server

A modular, RAII-safe C++20 HTTP server that efficiently handles client connections. Designed for learning modern C++ patterns, concurrency, and backend architecture.

## Features


- **RAII-safe Listener:** Socket properly managed via `FileDescriptor` for automatic cleanup when closed.

- **Multi-threading Ready:** Initial mono-threaded server for testing, easily extendable with a thread pool and a thread safe queue to handle high concurrency.

- **Robust HTTP Request Handling:** Server parses headers and bodies for both GET and POST requests, handles unexpected client disconnects gracefully, and remains stable under high load.

- **GET and POST Support:** Server handles GET requests to serve HTML, CSS, JS, PNG, JPG, and ICO files, provides POST endpoints /echo and /reverse and properly reads and logs POST data.

- **Path Sanitization & Security:** Protects against `../` directory traversal attacks with a path sanitizer.

- **Binary File Support:** Reads files in binary mode for images and scripts and sets proper `Content-Type` automatically based on file extension.

- **Graceful Shutdown:** Ctrl+C closes the server cleanly with no memory leaks or dangling sockets.

- **Easy Testing:** Compatible with `curl` or any browser.

## Build
`make`

## Run
`./httpServer 8080`

## Connect via curl or a browser:
### GET
```sh
curl http://localhost:8080/
curl http://localhost:8080/basic.html
curl http://localhost:8080/index.html
curl http://localhost:8080/style.css
curl http://localhost:8080/script.js
curl http://localhost:8080/images/logo.png
curl http://localhost:8080/favicon.ico
```
### POST
```sh
curl -d "Hello World" -X POST http://localhost:8080/
curl -d "Hello World" -X POST http://localhost:8080/echo
curl -d "Hello World" -X POST http://localhost:8080/reverse
```