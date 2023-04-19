#ifndef MY_CPP_SERVER_ACCEPTOR_HPP
#define MY_CPP_SERVER_ACCEPTOR_HPP

#include "my-cpp-server/utility/network.hpp"
#include "my-cpp-server/event/eventpoller.hpp"

class Acceptor {
public:
    Acceptor(Socket &server_socket, EventPoller &event_poller);
    // ~Acceptor();

    void accept();

private:
    Socket &server_socket_;
    EventPoller &event_poller_;
};

#endif //MY_CPP_SERVER_ACCEPTOR_HPP