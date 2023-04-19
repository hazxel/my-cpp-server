#ifndef MY_CPP_SERVER_EVENTLOOPER_HPP
#define MY_CPP_SERVER_EVENTLOOPER_HPP

#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/threadpool/threadpool.hpp"

class EventLooper {
public:
    EventLooper(EventPoller &event_poller, ThreadPool &thread_pool);
    // ~EventLooper();

    void loop();

private:
    EventPoller &event_poller_;
    ThreadPool &thread_pool_;
};

#endif //MY_CPP_SERVER_EVENTLOOPER_HPP