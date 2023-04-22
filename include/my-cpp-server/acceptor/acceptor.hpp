#ifndef MY_CPP_SERVER_ACCEPTOR_HPP
#define MY_CPP_SERVER_ACCEPTOR_HPP

#include "my-cpp-server/network/connection.hpp"
#include "my-cpp-server/event/eventpoller.hpp"

class Acceptor {
public:
    Acceptor(Connection &server_connection, EventPoller &event_poller);
    // ~Acceptor();

    void accept();

private:
    Connection &server_connection_;
    EventPoller &event_poller_;
};

#endif //MY_CPP_SERVER_ACCEPTOR_HPP