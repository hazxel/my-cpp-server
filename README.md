# my-cpp-server

### Project Structure

##### my-cpp-server

- acceptor: used to accept connection
- connection: support network connection, hide low-level protocal (TCP, UDP, QUIC, ...)
- event: support event poll and looping, hide system dependent API (epoll on Linux, kqueue on MacOS, ...)
- handler: handles the events
- server: server logics
- threadpool: manage threads
- utility: support error handling, logging, constants, etc.

##### executables

- server_run: start a server at default address 127.0.0.1 and default port 8080
- client_run: start a client and connect to the server
- test: start many clients to test the server


### Build
run `make` to build the project

### Todo
- switch to cmake, use macro to support different OS
- implement UDP
- implement QUIC