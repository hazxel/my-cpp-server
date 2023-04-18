#ifndef MY_SERVER_EPOLL_HPP
#define MY_SERVER_EPOLL_HPP

#include <sys/epoll.h>
#include <vector>

class Epoll {
public:
    Epoll();
    ~Epoll();

    void add_fd(int fd, uint32_t op);
    std::vector<epoll_event> poll(int max_events, int timeout);
    inline std::vector<epoll_event> poll_block(int max_events) { return poll(max_events, -1); }
    inline std::vector<epoll_event> poll_nonblock(int max_events) { return poll(max_events, 0); }

private:
    int epollfd_;
};

#endif // MY_SERVER_EPOLL_HPP