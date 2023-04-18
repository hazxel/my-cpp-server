#include <fcntl.h>
#include <cstring>

#include "error.hpp"
#include "network.hpp"

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