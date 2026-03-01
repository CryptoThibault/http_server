# C++20 HTTP Server

A modular, RAII-safe C++20 HTTP server that efficiently handles client connections. Designed for learning modern C++ patterns, concurrency, and backend architecture.

## Features

- Listener class with RAII-managed socket (FileDescriptor) ensuring proper cleanup
- Mono-threaded server for initial testing and prototyping
- Ready for multi-threaded architecture using a thread pool with ThreadSafeQueue
- Robust request parser (validates headers, handles disconnects) to avoid crashes during high concurrency
- Connection class capable of parsing minimal HTTP GET requests and sending responses
- Graceful shutdown on Ctrl+C, ensuring proper socket cleanup and no memory leaks

## Build
`make`

## Run
`./httpServer 8080`

## Connect via curl or a browser:

### GET
```sh
curl http://localhost:8080/
curl http://localhost:8080/hi
curl http://localhost:8080/about
curl http://localhost:8080/html
curl http://localhost:8080/c++
```

### POST
```sh
curl -d "Hello World" -X POST http://localhost:8080/
curl -d "Hello World" -X POST http://localhost:8080/echo
curl -d "Hello World" -X POST http://localhost:8080/reverse
```