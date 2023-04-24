#include <string>
#include <unistd.h>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <functional>

#include "my-cpp-server/threadpool/threadpool.hpp"
#include "my-cpp-server/network/socket.hpp"
#include "my-cpp-server/utility/constants.hpp"
#include "my-cpp-server/utility/error.hpp"

using namespace std;
using namespace constants;

void connectServer(int msg_num, int wait_time) {
    Socket client_socket;
    client_socket.connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    int sockfd = client_socket.get_fd();

    char send_buf[BUFFER_SIZE];
    memset(send_buf, 0, sizeof(send_buf));
    strcpy(send_buf, "hello!");

    char read_buf[BUFFER_SIZE];

    sleep(wait_time);


    for (int i = 0; i < msg_num; i++) {
        errif(write(sockfd, send_buf, sizeof(send_buf)) == -1, MSG_SOCKET_WRITE_ERR);   
        memset(read_buf, 0, sizeof(read_buf));
        ssize_t read_bytes = read(sockfd, read_buf, sizeof(read_buf));

        if (read_bytes > 0){
            cout << "message from server: " << read_buf << endl;
        } else if (read_bytes == 0) {
            cout << MSG_SERVER_SOCKET_DISCONNECTED << endl;
            break;
        } else if (read_bytes == -1) {
            errif(true, MSG_SOCKET_READ_ERR);
        }
    }
}

int main(int argc, char *argv[]) {
    int thread_num = 100;
    int msg_num = 100;
    int wait_time = 1000;

    int o;

    while ((o = getopt(argc, argv, "t:m:w:")) != -1) {
        switch (o) {
            case 't':
                thread_num = stoi(optarg);
                break;
            case 'm':
                msg_num = stoi(optarg);
                break;
            case 'w':
                wait_time = stoi(optarg);
                break;
            default:
                cout << "error optopt: " << optopt << endl;
                cout << "error opterr: " << opterr << endl;
                break;
        }
    }

    

    ThreadPool *thread_pool = new ThreadPool(thread_num);
    std::function<void()> func = std::bind(connectServer, msg_num, wait_time);
    for (int i = 0; i < thread_num; i++) {
        thread_pool->add_task(func);
    }

    sleep(wait_time+1);
    
    delete thread_pool;

    return 0;
}