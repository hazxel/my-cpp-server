#include <fcntl.h>

#include "error.hpp"
#include "network.hpp"

// set socket to non-blocking mode, only after connection is established
void set_nonblock(int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    errif(flags == -1, "fcntl get flags error");
    flags |= O_NONBLOCK;
    int ret = fcntl(fd, F_SETFL, flags);
    errif(ret == -1, "fcntl set flags error");
}