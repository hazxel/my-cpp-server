#include "my-cpp-server/server/server.hpp"
#include "my-cpp-server/utility/constants.hpp"
#include "my-cpp-server/network/socketconnection.hpp"
#include "my-cpp-server/event/eventpoller.hpp"
#include "my-cpp-server/event/eventlooper.hpp"
#include "my-cpp-server/acceptor/acceptor.hpp"
#include "my-cpp-server/threadpool/threadpool.hpp"


using namespace constants;

Server::Server() : pConnection_(new SocketConnection()), thread_pool_(), event_poller_(), event_looper_(event_poller_, thread_pool_), acceptor_(*pConnection_, event_poller_) {
    auto acceptor_callback = [&acceptor = acceptor_]() {acceptor.accept();};
    event_poller_.add_fd(pConnection_->get_fd(), acceptor_callback);
}

Server::~Server() {
    if (pConnection_ != nullptr) {
        delete pConnection_;
    }
}