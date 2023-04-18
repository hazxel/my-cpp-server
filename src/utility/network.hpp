#ifndef MY_SERVER_NETWORK_HPP
#define MY_SERVER_NETWORK_HPP

#include <arpa/inet.h>

void set_nonblock(int fd);

void construct_sockaddr_in(struct sockaddr_in& addr, const char* ip, int port);

#endif