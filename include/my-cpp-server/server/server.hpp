#ifndef MY_CPP_SERVER_SERVER_HPP
#define MY_CPP_SERVER_SERVER_HPP

#include "my-cpp-server/utility/network.hpp"
#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/event/eventlooper.hpp"
#include "my-cpp-server/acceptor/acceptor.hpp"
#include "my-cpp-server/threadpool/threadpool.hpp"

class Server {
public:
    Server();
    // ~Server();
    inline void run() { event_looper_.loop();}

private:
    Socket server_socket_;
    ThreadPool thread_pool_;
    EventPoller event_poller_;
    EventLooper event_looper_;  // take reference of event_poller_ and thread_pool_, must be initialized after them
    Acceptor acceptor_;         // take reference of event_poller_ and server_socket_, must be initialized after them
};

#endif //MY_CPP_SERVER_SERVER_HPP