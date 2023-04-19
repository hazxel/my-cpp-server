#include <vector>

#include "my-cpp-server/event/event.hpp"
#include "my-cpp-server/event/eventlooper.hpp"
#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/utility/constants.hpp"

using namespace constants;

EventLooper::EventLooper(EventPoller &event_poller) : event_poller_(event_poller) {}

void EventLooper::loop() {
    while (true) {
        std::vector<Event*> client_evs = event_poller_.poll_block(MAX_EVENTS);
        for (Event* ptr_ev : client_evs) {
            ptr_ev->handle();
        }
    }
}
