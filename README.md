# my-cpp-server



### Project Structure

my-cpp-server

- acceptor: used to accept connection
- connection: support network connection (TCP, QUIC, ...)
- event: support event poll and looping
- handler: handles the events
- server: server logics
- threadpool: manage threads
- utility: support error handling, logging, constants, etc.