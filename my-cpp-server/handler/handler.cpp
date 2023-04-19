#include <unistd.h>
#include <cstring>
#include <iostream>

#include "my-cpp-server/handler/handler.hpp"
#include "my-cpp-server/utility/error.hpp"
#include "my-cpp-server/utility/constants.hpp"


using namespace constants;
using std::cout;
using std::endl;

void handle_client_msg_et(int fd) {
    char buf[BUFFER_SIZE];
    while (true) {
        memset(buf, 0, BUFFER_SIZE);
        ssize_t read_bytes = read(fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            cout << "message from client fd " << fd << ": " << buf  << endl;
            write(fd, buf, sizeof(buf)); // send the same msg back to client
        } else if (read_bytes == 0) {
            cout << "client fd " << fd << " disconnected."  << endl;
            break;
        } else if (read_bytes == -1) {
            if (errno == EINTR) {
                cout << "client fd " << fd << " EINTR, continue reading..."  << endl;
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                cout << "client fd " << fd << " EAGAIN or EWOULDBLOCK, no data available now..."  << endl;
                break;
            } else {
                errif(true, MSG_SOCKET_READ_ERR);
            }
        }
    }
}

void handle_client_msg_lt(int fd) {
    char buf[BUFFER_SIZE];
    memset(buf, 0, BUFFER_SIZE);
    ssize_t read_bytes = read(fd, buf, sizeof(buf));
    if (read_bytes > 0) {
        cout << "message from client fd " << fd << ": " << buf  << endl;
        errif(write(fd, buf, sizeof(buf)) == -1, MSG_SOCKET_WRITE_ERR); 
    } else if (read_bytes == 0) {
        cout << "client fd " << fd << " disconnected."  << endl;
    } else if (read_bytes == -1) {
        errif(true, MSG_SOCKET_READ_ERR);
    }
}