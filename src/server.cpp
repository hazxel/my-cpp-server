#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <errno.h>

#include "utility/error.hpp"
#include "utility/network.hpp"
#include "utility/epoll.hpp"
#include "utility/constants.hpp"

using namespace constants;
using std::vector;
using std::cout;
using std::endl;

void handle_new_client(Epoll&, Socket&);
void handle_client_msg_et(int fd);
void handle_client_msg_lt(int fd);

int main() {
    Socket server_socket;
    int sockfd = server_socket.get_fd();
    server_socket.bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    server_socket.listen(SOMAXCONN);
    server_socket.set_nonblock(); // non-blocking mode because we use epoll at ET mode
    
    Epoll epoll;
    epoll.add_fd(sockfd, EPOLLIN);

    while (true) {
        vector<epoll_event> client_evs = epoll.poll_block(MAX_EVENTS);
        for (epoll_event ev : client_evs) {
            if (ev.data.fd == sockfd) {
                handle_new_client(epoll, server_socket);
            } else if (ev.events & EPOLLIN) {
                handle_client_msg_et(ev.data.fd);
            }
        }
    }
    return 0;
}

void handle_new_client(Epoll &epoll, Socket &server_socket) {
    int clnt_sockfd = server_socket.accept();
    set_nonblock(clnt_sockfd);
    epoll.add_fd(clnt_sockfd, EPOLLIN | EPOLLET); // ET mode
}

void handle_client_msg_et(int fd) {
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

void handle_client_msg_lt(int fd) {
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