#ifndef MY_CPP_SERVER_EVENTLOOPER_HPP
#define MY_CPP_SERVER_EVENTLOOPER_HPP

#include "my-cpp-server/event/eventpoller.hpp"

class EventLooper {
public:
    EventLooper(EventPoller &event_poller);
    // ~EventLooper();

    void loop();

private:
    EventPoller &event_poller_;
};

#endif //MY_CPP_SERVER_EVENTLOOPER_HPP