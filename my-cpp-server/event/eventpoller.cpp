#include <unistd.h>
#include <vector>
#include <memory>
#include <functional>

#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/event/event.hpp"
#include "my-cpp-server/utility/error.hpp"
#include "my-cpp-server/utility/constants.hpp"

using std::vector;
using namespace constants;

#ifdef MY_CPP_SERVER_PLATFORM_LINUX

#include <sys/epoll.h>
#include <cstring>


EventPoller::EventPoller() : fd_(-1) {
    fd_ = epoll_create(1024);
    errif(fd_ == -1, MSG_EPOLL_CREATE_ERR);
}

EventPoller::~EventPoller() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

void EventPoller::add_fd(int fd, EventType type, EventTriggerMode mode, const std::function<void()> &callback, bool involve_threadpool) {
    auto p_event = std::make_unique<Event>(fd, type, mode, involve_threadpool, callback);

    epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = p_event->get_epctl_flag();
    ev.data.ptr = p_event.get();

    events_.push_back(std::move(p_event));
    errif(epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &ev) == -1, MSG_EPOLL_CTL_ADD_ERR);
}

vector<Event*> EventPoller::poll(int max_events, int timeout) {
    epoll_event evs[max_events];
    int active_ev_num = epoll_wait(fd_, evs, max_events, timeout);
    errif(active_ev_num < 0, MSG_EPOLL_WAIT_ERR);

    vector<Event*> active_evs(active_ev_num);
    for (int i = 0; i < active_ev_num; ++i) {
        active_evs[i] = static_cast<Event*>(evs[i].data.ptr);
        active_evs[i]->set_epoll_event(evs[i].events);
    }
    return active_evs;
}

#endif // MY_CPP_SERVER_PLATFORM_LINUX


#ifdef MY_CPP_SERVER_PLATFORM_DARWIN

#include <sys/event.h>


EventPoller::EventPoller() : fd_(-1) {
    fd_ = kqueue();
    errif(fd_ == -1, MSG_KQUEUE_CREATE_ERR);
}

EventPoller::~EventPoller() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

void EventPoller::add_fd(int fd, EventType type, EventTriggerMode mode, const std::function<void()> &callback, bool involve_threadpool) {
    auto p_event = std::make_unique<Event>(fd, type, mode, involve_threadpool, callback);

    struct kevent ev;
    EV_SET(&ev, fd, p_event->get_filter_flag(), EV_ADD | p_event->get_trigger_flag(), 0, 0, p_event.get());

    events_.push_back(std::move(p_event));
    errif(kevent(fd_, &ev, 1, NULL, 0, NULL) == -1, MSG_KQUEUE_REGISTER_ERR);
}

vector<Event*> EventPoller::poll(int max_events, int timeout) {
    struct kevent evs[max_events];
    struct timespec ts;
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * 1000000;
    int active_ev_num = kevent(fd_, NULL, 0, evs, max_events, timeout == -1 ? NULL : &ts);
    errif(active_ev_num < 0, MSG_KQUEUE_POLL_ERR);

    vector<Event*> active_evs(active_ev_num);
    for (int i = 0; i < active_ev_num; ++i) {
        active_evs[i] = static_cast<Event*>(evs[i].udata);
    }
    return active_evs;
}

#endif // MY_CPP_SERVER_PLATFORM_DARWIN