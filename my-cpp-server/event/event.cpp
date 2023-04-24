#include <functional>

#include "my-cpp-server/event/event.hpp"

Event::Event(int fd, EventType type, EventTriggerMode trigger_mode, bool involve_thread_pool, std::function<void()> callback) : fd_(fd), ev_type_(type), ev_trigger_mode_(trigger_mode), involve_thread_pool_(involve_thread_pool), callback_(callback) {}