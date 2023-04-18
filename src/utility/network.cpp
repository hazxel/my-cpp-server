#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>


#include "error.hpp"
#include "network.hpp"
#include "constants.hpp"


using std::cout;
using std::endl;
using namespace constants;

// set socket to non-blocking mode, only after connection is established
void set_nonblock(int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    errif(flags == -1, "fcntl get flags error");
    flags |= O_NONBLOCK;
    int ret = fcntl(fd, F_SETFL, flags);
    errif(ret == -1, "fcntl set flags error");
}

void construct_sockaddr_in(struct sockaddr_in& addr, const char* ip, int port) {
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
}

Socket::Socket() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd_ == -1, MSG_SOCKET_CREATE_ERR);
    cout << "socket created! fd: " << sockfd_ << endl;
}

Socket::~Socket() {
    if (sockfd_ != -1) {
        close(sockfd_);
    }
}

void Socket::bind(const char* ip, int port) {
    construct_sockaddr_in(addr_, ip, port);
    errif(::bind(sockfd_, (sockaddr*) &addr_, sizeof(addr_)) == -1, MSG_SOCKET_BIND_ERR);
    cout << "socket binded! fd: " << sockfd_ << ", IP: " << inet_ntoa(addr_.sin_addr) << ", Port: " << ntohs(addr_.sin_port) << endl;
}

void Socket::listen(int backlog) {
    errif(::listen(sockfd_, backlog) == -1, MSG_SOCKET_LISTEN_ERR);
    cout << "socket listening..." << endl;
}

void Socket::connect(const char* ip, int port) {
    construct_sockaddr_in(addr_, ip, port);
    errif(::connect(sockfd_, (sockaddr*) &addr_, sizeof(addr_)) == -1, MSG_SOCKET_CONNECT_ERR);
    cout << "try to connect! fd: " << sockfd_ << ", server IP: " << inet_ntoa(addr_.sin_addr) << ", server Port: " << ntohs(addr_.sin_port) << endl;
}

int Socket::accept() {
    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sockfd = ::accept(sockfd_, (sockaddr*)&clnt_addr, &clnt_addr_len);
    errif(clnt_sockfd == -1, MSG_SOCKET_ACCEPT_ERR);
    cout << "accepted a new connection! client fd: " << clnt_sockfd << ", IP: " << inet_ntoa(clnt_addr.sin_addr) << ", Port: " << ntohs(clnt_addr.sin_port) << endl;
    return clnt_sockfd;
}

// set socket to non-blocking mode, only after connection is established
void Socket::set_nonblock() {
    ::set_nonblock(sockfd_);
}

