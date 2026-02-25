# C++20 HTTP Server

A modular, RAII-safe C++20 HTTP server that efficiently handles client connections. Designed for learning modern C++ patterns, concurrency, and backend architecture.

## Features

- Listener class with RAII-managed socket (`FileDescriptor`)
- Connection class for minimal HTTP GET request handling
- Mono-threaded server (Phase 2)
- Ready for multi-threaded architecture with thread pool (Phase 3)

## Build
make

## Run
./httpServer 8080

## Connect via `telnet` or a browser:
telnet 127.0.0.1 8080
GET / HTTP/1.1