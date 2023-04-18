#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "utility/error.hpp"
#include "utility/network.hpp"
#include "utility/constants.hpp"

using namespace constants;

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, MSG_SOCKET_CREATE_ERR);

    struct sockaddr_in serv_addr;
    construct_sockaddr_in(serv_addr, DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    connect(sockfd, (sockaddr*) &serv_addr, sizeof(serv_addr));

    while(true){
        char buf[BUFFER_SIZE];
        memset(buf, 0, sizeof(buf));
        std::cin >> buf;
        errif(write(sockfd, buf, sizeof(buf)) == -1, MSG_SOCKET_WRITE_ERR);   

        memset(buf, 0, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));

        if (read_bytes > 0){
            std::cout << "message from server: " << buf << std::endl;
        } else if (read_bytes == 0) {
            std::cout << MSG_SOCKET_DISCONNECTED << std::endl;
            break;
        } else if (read_bytes == -1) {
            close(sockfd);
            errif(true, MSG_SOCKET_READ_ERR);
        }
    }

    return 0;
}