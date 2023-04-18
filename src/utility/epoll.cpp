#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <vector>

#include "epoll.hpp"
#include "error.hpp"
#include "constants.hpp"

using std::vector;
using namespace constants;

Epoll::Epoll() {
    epollfd_ = epoll_create(1024);
    errif(epollfd_ == -1, MSG_EPOLL_CREATE_ERR);
}

Epoll::~Epoll() {
    if (epollfd_ != -1) {
        close(epollfd_);
    }
}

void Epoll::add_fd(int fd, uint32_t op) {
    epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = op;
    ev.data.fd = fd;
    errif(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1, MSG_EPOLL_CTL_ADD_ERR);
}

vector<epoll_event> Epoll::poll(int max_events, int timeout) {
    vector<epoll_event> evs(max_events);
    int ready_events = epoll_wait(epollfd_, evs.data(), max_events, timeout);
    errif(ready_events == -1, MSG_EPOLL_WAIT_ERR);
    evs.resize(ready_events);
    return evs;
}