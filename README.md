# C++20 HTTP Server

A modular, RAII-safe C++20 HTTP server that efficiently handles client connections. Designed for learning modern C++ patterns, concurrency, and backend architecture.

## Features

- Listener class with RAII-managed socket (FileDescriptor) ensuring proper cleanup
- Connection class capable of parsing minimal HTTP GET requests and sending responses
- Mono-threaded server for initial testing and prototyping
- Ready for multi-threaded architecture using a thread pool with ThreadSafeQueue

## Build
make

## Run
./httpServer 8080

## Connect via `telnet` or a browser:
telnet 127.0.0.1 8080
GET / HTTP/1.1
