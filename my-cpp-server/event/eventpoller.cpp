#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <memory>
#include <functional>

#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/event/event.hpp"
#include "my-cpp-server/utility/error.hpp"
#include "my-cpp-server/utility/constants.hpp"

using std::vector;
using namespace constants;

EventPoller::EventPoller() : epollfd_(-1) {
    epollfd_ = epoll_create(1024);
    errif(epollfd_ == -1, MSG_EPOLL_CREATE_ERR);
}

EventPoller::~EventPoller() {
    if (epollfd_ != -1) {
        close(epollfd_);
        epollfd_ = -1;
    }
}

void EventPoller::add_fd(int fd, const std::function<void()> &callback) {
    auto p_event = std::make_unique<Event>(fd, EventType::READ, EventTriggerMode::EDGE, callback);

    epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = p_event->get_epctl_flag();
    ev.data.ptr = p_event.get();

    events_.push_back(std::move(p_event));
    errif(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1, MSG_EPOLL_CTL_ADD_ERR);
}

vector<Event*> EventPoller::poll(int max_events, int timeout) {
    epoll_event evs[max_events];
    int active_ev_num = epoll_wait(epollfd_, evs, max_events, timeout);
    errif(active_ev_num < 0, MSG_EPOLL_WAIT_ERR);

    vector<Event*> active_evs(active_ev_num);
    for (int i = 0; i < active_ev_num; ++i) {
        active_evs[i] = static_cast<Event*>(evs[i].data.ptr);
        active_evs[i]->set_epoll_event(evs[i].events);
    }
    return active_evs;
}