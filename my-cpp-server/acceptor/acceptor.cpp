#include <thread>

#include "my-cpp-server/acceptor/acceptor.hpp"
#include "my-cpp-server/utility/network.hpp"
#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/handler/handler.hpp"

Acceptor::Acceptor(Socket &server_socket, EventPoller &event_poller) : server_socket_(server_socket), event_poller_(event_poller) {}

void Acceptor::accept() {
    int clnt_sockfd = server_socket_.accept();
    set_nonblock(clnt_sockfd);
    auto callback = std::bind(handle_client_msg_et, clnt_sockfd);
    event_poller_.add_fd(clnt_sockfd, callback); // ET mode inside 
}

