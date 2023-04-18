#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <errno.h>

#include "utility/error.hpp"
#include "utility/network.hpp"

#define BUFFER_SIZE 1024
#define MAX_EVENTS 1024

void handle_new_client(int epfd, int sockfd);
void handle_client_msg_et(int fd);
void handle_client_msg_lt(int fd);

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr*) &serv_addr, sizeof(serv_addr)) == -1, "socket bind error");
    std::cout << "Server socket created! fd: " << sockfd << ", IP: " << inet_ntoa(serv_addr.sin_addr) << ", Port: " << ntohs(serv_addr.sin_port) << std::endl;

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");
    std::cout << "Server socket listening..." << std::endl;

    int epfd = epoll_create(1024);
    errif(epfd == -1, "epoll create error");

    epoll_event client_evs[MAX_EVENTS], server_ev;
    server_ev.events = EPOLLIN;
    server_ev.data.fd = sockfd;

    set_nonblock(sockfd); // non-blocking mode because we use epoll at ET mode
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &server_ev) == -1, "epoll ctl add error"); 

    while (true) {
        int ready_events = epoll_wait(epfd, client_evs, MAX_EVENTS, -1);
        errif(ready_events == -1, "epoll wait error");

        for (int i = 0; i < ready_events; ++i) {
            if (client_evs[i].data.fd == sockfd) {
                handle_new_client(epfd, sockfd);
            } else if (client_evs[i].events & EPOLLIN) {
                handle_client_msg_et(client_evs[i].data.fd);
            }
        }
    }
    
    return 0;
}

void handle_new_client(int epfd, int sockfd) {
    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    std::cout << "Accepted a new client! fd: " << clnt_sockfd << ", IP: " << inet_ntoa(clnt_addr.sin_addr) << ", Port: " << ntohs(clnt_addr.sin_port) << std::endl;
    epoll_event client_ev;
    client_ev.events = EPOLLIN | EPOLLET; // ET mode
    client_ev.data.fd = clnt_sockfd;
    set_nonblock(clnt_sockfd);
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &client_ev) == -1, "epoll ctl add error");
}

void handle_client_msg_et(int fd) {
    char buf[BUFFER_SIZE];
    while (true) {
        memset(buf, 0, BUFFER_SIZE);
        ssize_t read_bytes = read(fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            std::cout << "message from client fd " << fd << ": " << buf  << std::endl;
            write(fd, buf, sizeof(buf)); // send the same msg back to client
        } else if (read_bytes == 0) {
            std::cout << "client fd " << fd << " disconnected."  << std::endl;
            close(fd);
            break;
        } else if (read_bytes == -1) {
            if (errno == EINTR) {
                std::cout << "client fd " << fd << " EINTR, continue reading..."  << std::endl;
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cout << "client fd " << fd << " EAGAIN or EWOULDBLOCK, no data available now..."  << std::endl;
                break;
            } else {
                close(fd);
                errif(true, "socket read error");
            }
        }
    }
}

void handle_client_msg_lt(int fd) {
    char buf[BUFFER_SIZE];
    memset(buf, 0, BUFFER_SIZE);
    ssize_t read_bytes = read(fd, buf, sizeof(buf));
    if (read_bytes > 0) {
        std::cout << "message from client fd " << fd << ": " << buf  << std::endl;
        write(fd, buf, sizeof(buf)); 
    } else if (read_bytes == 0) {
        std::cout << "client fd " << fd << " disconnected."  << std::endl;
        close(fd);
    } else if (read_bytes == -1) {
        close(fd);
        errif(true, "socket read error");
    }
}