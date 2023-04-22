#include <thread>

#include "my-cpp-server/acceptor/acceptor.hpp"
#include "my-cpp-server/network/socket.hpp"
#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/handler/handler.hpp"

Acceptor::Acceptor(Connection &connection, EventPoller &event_poller) : server_connection_(connection), event_poller_(event_poller) {}

void Acceptor::accept() {
    int clnt_sockfd = server_connection_.accept();
    set_nonblock(clnt_sockfd);
    auto callback = std::bind(handle_client_msg_et, clnt_sockfd);
    event_poller_.add_fd(clnt_sockfd, callback); // ET mode inside 
}

