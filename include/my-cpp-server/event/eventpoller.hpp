#ifndef MY_SERVER_EPOLL_HPP
#define MY_SERVER_EPOLL_HPP

#include <vector>
#include <memory>
#include <functional>

#include "my-cpp-server/event/event.hpp"

class EventPoller {
public:
    EventPoller();
    ~EventPoller();

    void add_fd(int fd, EventType type, EventTriggerMode mode, const std::function<void()> &callback, bool use_threadpool);
    inline void add_fd_read(int fd, EventTriggerMode mode, const std::function<void()> &callback, bool use_threadpool) {
        add_fd(fd, EventType::READ, mode, callback, use_threadpool);}
    inline void add_fd_write(int fd, EventTriggerMode mode, const std::function<void()> &callback, bool use_threadpool) {
        add_fd(fd, EventType::WRITE, mode, callback, use_threadpool);}
        
    std::vector<Event*> poll(int max_events, int timeout);
    inline std::vector<Event*> poll_block(int max_events) { return poll(max_events, -1); }
    inline std::vector<Event*> poll_nonblock(int max_events) { return poll(max_events, 0); }

private:
    int fd_;
    std::vector<std::unique_ptr<Event>> events_;
};


#endif // MY_SERVER_EPOLL_HPP