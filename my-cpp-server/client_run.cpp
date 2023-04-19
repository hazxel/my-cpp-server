#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "my-cpp-server/utility/error.hpp"
#include "my-cpp-server/utility/network.hpp"
#include "my-cpp-server/utility/constants.hpp"

using namespace constants;

int main() {
    Socket client_socket;
    client_socket.connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    int sockfd = client_socket.get_fd();

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
            std::cout << MSG_SERVER_SOCKET_DISCONNECTED << std::endl;
            break;
        } else if (read_bytes == -1) {
            errif(true, MSG_SOCKET_READ_ERR);
        }
    }

    return 0;
}