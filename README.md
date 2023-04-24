# my-cpp-server



### Project Structure

my-cpp-server

- acceptor: used to accept connection
- connection: support network connection, hide low-level protocal (TCP, UDP, QUIC, ...)
- event: support event poll and looping, hide system dependent API (epoll, kqueue, ...)
- handler: handles the events
- server: server logics
- threadpool: manage threads
- utility: support error handling, logging, constants, etc.

### Todo
- add test
- switch to cmake, use macro to support different OS
- using kqueue on macOS
- implement UDP
- implement QUIC