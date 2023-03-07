#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

#include "error.hpp"

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    bind(sockfd, (sockaddr*) &serv_addr, sizeof(serv_addr));

    std::cout << "Server socket created! fd: " << sockfd << ", IP: " << inet_ntoa(serv_addr.sin_addr) << ", Port: " << ntohs(serv_addr.sin_port) << std::endl;

    listen(sockfd, SOMAXCONN);

    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    
    std:: cout << "Accepted a new client! fd: " << clnt_sockfd << ", IP: " << inet_ntoa(clnt_addr.sin_addr) << ", Port: " << ntohs(clnt_addr.sin_port) << std::endl;

    return 0;
}