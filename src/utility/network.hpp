#ifndef MY_SERVER_NETWORK_HPP
#define MY_SERVER_NETWORK_HPP

#include <arpa/inet.h>

void set_nonblock(int fd);

void construct_sockaddr_in(struct sockaddr_in& addr, const char* ip, int port);

class Socket {
public:
    Socket();
    ~Socket();

    void bind(const char* ip, int port);
    void listen(int backlog);
    void connect(const char* ip, int port);
    int accept();

    void set_nonblock();

    inline int get_fd() const { return sockfd_; }
private:
    int sockfd_;
    struct sockaddr_in addr_;
};


#endif // MY_SERVER_NETWORK_HPP