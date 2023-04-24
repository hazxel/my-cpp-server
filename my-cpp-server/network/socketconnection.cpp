#include "my-cpp-server/network/socketconnection.hpp"
#include "my-cpp-server/utility/constants.hpp"

using namespace constants;

SocketConnection::SocketConnection() : socket_() {
    socket_.bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    socket_.listen(SOMAXCONN);
    //socket_.set_nonblock(); // non-blocking mode if use epoll at ET mode
}