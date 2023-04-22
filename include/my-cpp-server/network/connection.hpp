#ifndef MY_CPP_SERVER_CONNECTION_HPP
#define MY_CPP_SERVER_CONNECTION_HPP


#include "my-cpp-server/network/socket.hpp"

class Connection {
public:
    Connection() {}
    virtual ~Connection() = 0;

    virtual int get_fd() const = 0;
    virtual int accept() = 0;
};

inline Connection::~Connection() {}

#endif //MY_CPP_SERVER_CONNECTION_HPP