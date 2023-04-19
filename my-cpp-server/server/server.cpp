#include <unistd.h>
#include <cstring>
#include <iostream>

#include "my-cpp-server/server/server.hpp"
#include "my-cpp-server/utility/error.hpp"
#include "my-cpp-server/utility/network.hpp"
#include "my-cpp-server/utility/constants.hpp"
#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/event/eventlooper.hpp"


using std::cout;
using std::endl;
using namespace constants;

Server::Server() : server_socket_(Socket()), event_poller_(EventPoller()), event_looper_(EventLooper(event_poller_)) {
    int sockfd = server_socket_.get_fd();
    server_socket_.bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    server_socket_.listen(SOMAXCONN);
    server_socket_.set_nonblock(); // non-blocking mode because we use epoll at ET mode
    
    auto callback = std::bind(handle_new_client, &event_poller_, &server_socket_);
    event_poller_.add_fd(sockfd, callback);
}


void Server::handle_new_client(EventPoller *ep, Socket *server_socket) {
    int clnt_sockfd = server_socket->accept();
    set_nonblock(clnt_sockfd);
    auto callback = std::bind(handle_client_msg_lt, clnt_sockfd);
    ep->add_fd(clnt_sockfd, callback); // ET mode inside 
}

void Server::handle_client_msg_et(int fd) {
    char buf[BUFFER_SIZE];
    while (true) {
        memset(buf, 0, BUFFER_SIZE);
        ssize_t read_bytes = read(fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            cout << "message from client fd " << fd << ": " << buf  << endl;
            write(fd, buf, sizeof(buf)); // send the same msg back to client
        } else if (read_bytes == 0) {
            cout << "client fd " << fd << " disconnected."  << endl;
            break;
        } else if (read_bytes == -1) {
            if (errno == EINTR) {
                cout << "client fd " << fd << " EINTR, continue reading..."  << endl;
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                cout << "client fd " << fd << " EAGAIN or EWOULDBLOCK, no data available now..."  << endl;
                break;
            } else {
                errif(true, MSG_SOCKET_READ_ERR);
            }
        }
    }
}

void Server::handle_client_msg_lt(int fd) {
    char buf[BUFFER_SIZE];
    memset(buf, 0, BUFFER_SIZE);
    ssize_t read_bytes = read(fd, buf, sizeof(buf));
    if (read_bytes > 0) {
        cout << "message from client fd " << fd << ": " << buf  << endl;
        errif(write(fd, buf, sizeof(buf)) == -1, MSG_SOCKET_WRITE_ERR); 
    } else if (read_bytes == 0) {
        cout << "client fd " << fd << " disconnected."  << endl;
    } else if (read_bytes == -1) {
        errif(true, MSG_SOCKET_READ_ERR);
    }
}