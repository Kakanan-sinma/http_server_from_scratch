# HTTP Server From Scratch (OSI Layer 7 Deep Dive)

A lightweight HTTP/1.1 web server implemented in C from scratch to study **OSI Model Layer 7 (Application Layer)** and **TCP Sockets (Layer 4)** on Linux.

## Project Architecture
- 'src/core/': Layer 4 TCP Socket Handling (socket, bind, listen, accept)
- 'src/http/': Layer 7 Protocol Parsing & Building (Request Header Parser, Response Formatter)
- 'src/router/': Path Routing & File Handler
- 'public/': Static Web Assets

## Roadmap
- [ ] **Phase 1**: TCP Server Listener (Layer 4)
- [ ] **Phase 2**: HTTP Request Parsing (Method, Path, Headers)
- [ ] **Phase 3**: HTTP Response Generation (200 OK, Status Codes)
- [ ] **Phase 4**: Static File Serving & Routing

