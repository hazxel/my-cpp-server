#ifndef MY_SERVER_EPOLL_HPP
#define MY_SERVER_EPOLL_HPP

#include <sys/epoll.h>
#include <vector>
#include <memory>
#include <functional>

#include "my-cpp-server/event/event.hpp"

class EventPoller {
public:
    EventPoller();
    ~EventPoller();

    void add_fd(int fd, const std::function<void()> &callback);
    std::vector<Event*> poll(int max_events, int timeout);
    inline std::vector<Event*> poll_block(int max_events) { return poll(max_events, -1); }
    inline std::vector<Event*> poll_nonblock(int max_events) { return poll(max_events, 0); }

private:
    int epollfd_;
    std::vector<std::unique_ptr<Event>> events_;
};


#endif // MY_SERVER_EPOLL_HPP