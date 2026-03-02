# C++20 HTTP Server

A modular, RAII-safe C++20 HTTP server that efficiently handles client connections. Designed for learning modern C++ patterns, concurrency, and backend architecture.

## Features


- **RAII-safe Listener**  
  Socket properly managed via `FileDescriptor` for automatic cleanup when closed.

- **Multi-threading Ready**  
  Initial mono-threaded server for testing, easily extendable with a **thread pool** and **ThreadSafeQueue** to handle high concurrency.

- **Robust HTTP Request Handling**  
  - Parses headers and body for GET and POST requests  
  - Handles unexpected client disconnects  
  - Stable under high load (`ab -n 100000 -c 1000` tested)

- **GET and POST Support**  
  - GET: serves HTML, CSS, JS, PNG, JPG, and ICO files  
  - POST: `/echo` and `/reverse` endpoints for testing request body  
  - Properly reads and logs POST bodies

- **Path Sanitization & Security**  
  - Protects against `../` directory traversal attacks  
  - Uses `std::filesystem::canonical` to validate paths

- **Binary File Support**  
  - Reads files in binary mode for images and scripts  
  - Sets proper `Content-Type` automatically based on file extension

- **Interactive Web Page**  
  - Responsive HTML page with title, paragraph, button, and image  
  - “Show/Hide Image” button controlled with JavaScript  
  - Modern CSS layout, centered and mobile-friendly

- **Favicon Support**  
  - Serves `favicon.ico` correctly in the browser tab  
  - Proper `Content-Type: image/x-icon`

- **Graceful Shutdown**  
  - Ctrl+C closes the server cleanly  
  - No memory leaks or dangling sockets

- **Easy Testing**  
  - Compatible with `curl` or any browser  
  - Example GET and POST requests included

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