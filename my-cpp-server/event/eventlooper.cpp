#include <vector>

#include "my-cpp-server/event/event.hpp"
#include "my-cpp-server/event/eventlooper.hpp"
#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/threadpool/threadpool.hpp"
#include "my-cpp-server/utility/constants.hpp"

using namespace constants;

EventLooper::EventLooper(EventPoller &event_poller, ThreadPool &thread_pool) : event_poller_(event_poller), thread_pool_(thread_pool) {}

void EventLooper::loop() {
    while (true) {
        std::vector<Event*> client_evs = event_poller_.poll_block(MAX_EVENTS);
        for (Event* ptr_ev : client_evs) {
            thread_pool_.add_task(ptr_ev->get_callback());
        }
    }
}
