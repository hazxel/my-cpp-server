#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "utility/error.hpp"
#include "utility/network.hpp"

#define BUFFER_SIZE 1024


int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    connect(sockfd, (sockaddr*) &serv_addr, sizeof(serv_addr));

    while(true){
        char buf[BUFFER_SIZE];
        memset(buf, 0, sizeof(buf));
        std::cin >> buf;
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));      
        if (write_bytes == -1) {        
            std::cout << "socket write failure, maybe disconnected with server!" << std::endl;
            break;
        }
        memset(buf, 0, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));

        if (read_bytes > 0){
            std::cout << "message from server:" << buf << std::endl;
        } else if (read_bytes == 0) {
            std::cout << "server socket disconnected!" << buf << std::endl;
            break;
        } else if (read_bytes == -1) {
            close(sockfd);
            errif(true, "socket read error");
        }
    }

    return 0;
}