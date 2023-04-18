#ifndef MY_SERVER_CONSTANTS_HPP
#define MY_SERVER_CONSTANTS_HPP

namespace constants {
    const int   BUFFER_SIZE = 1024;
    const int   MAX_EVENTS = 1024;
    const int   MAX_CONNECTIONS = 1024;

    const char* DEFAULT_SERVER_IP = "127.0.0.1";
    const int   DEFAULT_SERVER_PORT = 8888;

    const char* MSG_SOCKET_CREATE_ERR = "socket create error.";
    const char* MSG_SOCKET_BIND_ERR = "socket bind error.";
    const char* MSG_SOCKET_LISTEN_ERR = "socket listen error.";
    const char* MSG_SOCKET_ACCEPT_ERR = "socket accept error.";
    const char* MSG_SOCKET_READ_ERR = "socket read error.";
    const char* MSG_SOCKET_WRITE_ERR = "socket write error, connection may be closed.";
    const char* MSG_SOCKET_DISCONNECTED = "socket disconnected.";

    const char* MSG_EPOLL_CREATE_ERR = "epoll create error.";
    const char* MSG_EPOLL_CTL_ADD_ERR = "epoll ctl add error.";
    const char* MSG_EPOLL_WAIT_ERR = "epoll wait error.";
}

#endif