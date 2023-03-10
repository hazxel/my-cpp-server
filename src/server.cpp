#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "error.hpp"

#define BUFFER_SIZE 1024

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr*) &serv_addr, sizeof(serv_addr)), "socket bind error");

    std::cout << "Server socket created! fd: " << sockfd << ", IP: " << inet_ntoa(serv_addr.sin_addr) << ", Port: " << ntohs(serv_addr.sin_port) << std::endl;

    errif(listen(sockfd, SOMAXCONN), "socket listen error");

    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    
    std::cout << "Accepted a new client! fd: " << clnt_sockfd << ", IP: " << inet_ntoa(clnt_addr.sin_addr) << ", Port: " << ntohs(clnt_addr.sin_port) << std::endl;

    while (true) {
        char buf[BUFFER_SIZE];
        memset(buf, 0, BUFFER_SIZE);

        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));

        if (read_bytes > 0) {
            std::cout << "message from client fd " << clnt_sockfd << ": " << buf  << std::endl;
            write(clnt_sockfd, buf, sizeof(buf)); 
        } else if (read_bytes == 0) {
            std::cout << "client fd " << clnt_sockfd << " disconnected."  << std::endl;
            close(clnt_sockfd);
            break;
        } else if (read_bytes == -1) {
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
    }
    
    return 0;
}