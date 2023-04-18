#ifndef MY_SERVER_CONSTANTS_HPP
#define MY_SERVER_CONSTANTS_HPP

namespace constants {
    inline const int  BUFFER_SIZE = 1024;
    inline const int  MAX_EVENTS = 1024;
    inline const int  MAX_CONNECTIONS = 1024;

    inline const char DEFAULT_SERVER_IP[] = "127.0.0.1";
    inline const int  DEFAULT_SERVER_PORT = 8888;

    inline const char MSG_SOCKET_CREATE_ERR[] = "socket create error.";
    inline const char MSG_SOCKET_BIND_ERR[] = "socket bind error.";
    inline const char MSG_SOCKET_LISTEN_ERR[] = "socket listen error.";
    inline const char MSG_SOCKET_CONNECT_ERR[] = "socket connect error.";
    inline const char MSG_SOCKET_ACCEPT_ERR[] = "socket accept error.";
    inline const char MSG_SOCKET_READ_ERR[] = "socket read error.";
    inline const char MSG_SOCKET_WRITE_ERR[] = "socket write error, connection may be closed.";
    inline const char MSG_SERVER_SOCKET_DISCONNECTED[] = "server socket disconnected.";

    inline const char MSG_EPOLL_CREATE_ERR[] = "epoll create error.";
    inline const char MSG_EPOLL_CTL_ADD_ERR[] = "epoll ctl add error.";
    inline const char MSG_EPOLL_WAIT_ERR[] = "epoll wait error.";
}

#endif // MY_SERVER_CONSTANTS_HPP