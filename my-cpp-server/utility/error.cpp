#include <errno.h>
#include <iostream>
#include "my-cpp-server/utility/error.hpp"

void errif(bool condition, const char *errmsg) {
    if(condition){
      	std::cerr << errmsg << " (errno=" << errno << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
}