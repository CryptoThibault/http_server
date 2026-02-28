# C++20 HTTP Server

A modular, RAII-safe C++20 HTTP server that efficiently handles client connections. Designed for learning modern C++ patterns, concurrency, and backend architecture.

## Features

- Listener class with RAII-managed socket (FileDescriptor) ensuring proper cleanup
- Connection class capable of parsing minimal HTTP GET requests and sending responses
- Mono-threaded server for initial testing and prototyping
- Ready for multi-threaded architecture using a thread pool with ThreadSafeQueue
- Support for HTTP GET and POST methods with multiple routes, and custom responses

## Build
`make`

## Run
`./httpServer 8080`

## Connect via curl or a browser:

### GET
`curl http://localhost:8080/`
`curl http://localhost:8080/hi`
`curl http://localhost:8080/about`
`curl http://localhost:8080/html`
`curl http://localhost:8080/c++`

### POST
`curl -d "Hello World" -X POST http://localhost:8080/`
`curl -d "Hello World" -X POST http://localhost:8080/echo`
`curl -d "Hello World" -X POST http://localhost:8080/reverse`