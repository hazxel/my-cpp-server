#include <iostream>

void errif(bool condition, const char *errmsg){
    if(condition){
      	std::cerr << errmsg << std::endl;
        exit(EXIT_FAILURE);
    }
}