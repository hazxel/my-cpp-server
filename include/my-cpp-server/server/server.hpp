#ifndef MY_CPP_SERVER_SERVER_HPP
#define MY_CPP_SERVER_SERVER_HPP

#include "my-cpp-server/utility/network.hpp"
#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/event/eventlooper.hpp"

class Server {
public:
    Server();
    // ~Server();
    inline void run() { event_looper_.loop();}

private:
    static void handle_new_client(EventPoller *ep, Socket *server_socket);
    static void handle_client_msg_et(int fd);
    static void handle_client_msg_lt(int fd);

    Socket server_socket_;
    EventPoller event_poller_;
    EventLooper event_looper_;
};

#endif //MY_CPP_SERVER_SERVER_HPP