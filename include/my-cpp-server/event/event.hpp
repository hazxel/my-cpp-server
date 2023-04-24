#ifndef MY_CPP_SERVER_EVENT_HPP
#define MY_CPP_SERVER_EVENT_HPP


#ifdef MY_CPP_SERVER_PLATFORM_LINUX

#include <sys/epoll.h>
#include <cstdint>
#include <functional>


enum class EventType : uint32_t {
    READ = EPOLLIN,
    WRITE = EPOLLOUT,
    // ERROR = EPOLLERR,
    // HUP = EPOLLHUP,
    // ONESHOT = EPOLLONESHOT
};

enum class EventTriggerMode : uint32_t {
    LEVEL = 0,
    EDGE = EPOLLET
};

class Event {
public:
    Event(int fd, EventType ev_type, EventTriggerMode ev_trigger_mode, bool involve_thread_pool, std::function<void()> callback);
    // ~Event();

    inline int get_fd() const { return fd_; }
    inline uint32_t get_ev() const { return epoll_events_; }
    inline std::function<void()> get_callback() const { return callback_; }
    inline uint32_t get_epctl_flag() const { return static_cast<uint32_t>(ev_type_) | static_cast<uint32_t>(ev_trigger_mode_); }
    inline void set_epoll_event(uint32_t ev) { epoll_events_ = ev; }
    inline bool involve_thread_pool() const { return involve_thread_pool_; }

private:
    int fd_;
    EventType ev_type_;
    EventTriggerMode ev_trigger_mode_;
    uint32_t epoll_events_;
    bool involve_thread_pool_;
    std::function<void()> callback_;
};

#endif // MY_CPP_SERVER_PLATFORM_LINUX


#ifdef MY_CPP_SERVER_PLATFORM_DARWIN

#include <sys/event.h>
#include <functional>


enum class EventType : int {
    READ = EVFILT_READ,
    WRITE = EVFILT_WRITE
    // ERROR,
    // HUP,
    // ONESHOT
};

enum class EventTriggerMode : int {
    LEVEL = 0,
    EDGE = EV_CLEAR
};
class Event {
public:
    Event(int fd, EventType ev_type, EventTriggerMode ev_trigger_mode, bool involve_thread_pool, std::function<void()> callback);
    
    inline int get_fd() const { return fd_; }
    inline std::function<void()> get_callback() const { return callback_; }
    inline int get_filter_flag() const { return static_cast<int>(ev_type_); }
    inline int get_trigger_flag() const { return static_cast<int>(ev_trigger_mode_); }
    inline bool involve_thread_pool() const { return involve_thread_pool_; }

private:
    int fd_;
    EventType ev_type_;
    EventTriggerMode ev_trigger_mode_;
    bool involve_thread_pool_;
    std::function<void()> callback_;
};

#endif // MY_CPP_SERVER_PLATFORM_DARWIN

#endif //MY_CPP_SERVER_EVENT_HPP