#include "my-cpp-server/server/server.hpp"
#include "my-cpp-server/utility/network.hpp"
#include "my-cpp-server/utility/constants.hpp"
#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/event/eventlooper.hpp"
#include "my-cpp-server/acceptor/acceptor.hpp"
#include "my-cpp-server/threadpool/threadpool.hpp"


using namespace constants;

Server::Server() : server_socket_(), thread_pool_(), event_poller_(), event_looper_(event_poller_, thread_pool_), acceptor_(server_socket_, event_poller_) {
    int sockfd = server_socket_.get_fd();
    server_socket_.bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    server_socket_.listen(SOMAXCONN);
    server_socket_.set_nonblock(); // non-blocking mode because we use epoll at ET mode
    
    auto acceptor_callback = [&acceptor = acceptor_] {acceptor.accept();};
    event_poller_.add_fd(sockfd, acceptor_callback);
}