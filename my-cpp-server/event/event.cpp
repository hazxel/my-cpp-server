#include <functional>

#include "my-cpp-server/event/event.hpp"

Event::Event(int fd, EventType type, EventTriggerMode trigger_mode, std::function<void()> callback) : fd_(fd), ev_type_(type), ev_trigger_mode_(trigger_mode), callback_(callback) {}