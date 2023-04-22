#ifndef MY_CPP_SERVER_SOCKET_CONNECTION_HPP
#define MY_CPP_SERVER_SOCKET_CONNECTION_HPP

#include "my-cpp-server/network/connection.hpp"
#include "my-cpp-server/network/socket.hpp"


class SocketConnection : public Connection {
public:
    SocketConnection();
    ~SocketConnection() override {}

    inline int get_fd() const override { return socket_.get_fd(); }
    inline int accept() override { return socket_.accept(); }
private:
    Socket socket_;
};


#endif //MY_CPP_SERVER_SOCKET_CONNECTION_HPP