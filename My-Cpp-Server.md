# My Cpp Server

### Socket - \<sys/socket.h\>

##### socket - create an endpoint for communication 

```c++
int socket(int domain, int type, int protocol);
```

*Argumemts:* 

1. domain: IP address type, `AF_INET` for IPv4, `AF_INET6` for IPv6, `AF_UNIX` for file system pathnames 
2. type:  the semantics of communication, possible types are:

- `SOCK_STREAM`: sequenced, reliable, bidirectional, connection-mode byte streams
- `SOCK_DGRAM`: datagrams -- connectionless-mode, unreliable messages of fixed maximum length
- `SOCK_SEQPACKET`: sequenced, reliable, bidirectional, connection-mode transmission path for records.

3. protocol: `IPPROTO_TCP`, `IPPTOTO_UDP` or `0` to use a default protocol for the requested socket type.

*Return value:*

Upon successful completion, *socket()* returns a **nonnegative integer**, the **socket file descriptor**. Otherwise a value of -1 is returned and *errno* is set to indicate the error.



##### bind - assigns an address to an unnamed socket

```c++
int bind(int socket, const struct sockaddr *address, socklen_t address_len);
```

*Arguments*:

1. socket: the file descriptor of the socket to be bound
2. address: points to a `sockaddr` structure containing the address to be bound to the socket
3. address_len: specifies the length of the `sockaddr` structure

*Return value:*

Upon successful completion, returns 0, otherwise -1 is returned and *errno* is set to indicate the error.

> `sockaddr_in` vs `sockaddr`: struct `sockaddr` is a general structure valid for any protocol. `sockaddr_in` is specific for IPv4 address family.

> Usually, only server side need to bind a port because: bind可以提前确定端口号，如浏览网页服务的80端口，用于FTP服务的21端口等，client基本没这个需求



##### listen - wait for socket connections and limit the queue of incoming connections

```c++
int listen(int socket, int backlog);
```

*Arguments*:

1. socket: the file descriptor specifying a **connection-mode** socket
2. backlog: a positive number limiting the number of outstanding connections in the listen queue. The suggested max value `SOMAXCONN` is 128.

*Return value:*

Upon successful completion, returns 0, otherwise -1 is returned and *errno* is set to indicate the error.



##### connect - requests a connection to be made on a socket

```c++
int connect(int socket, const struct sockaddr *address, socklen_t address_len);
```

Arguments*:

1. socket: the file descriptor specifying a **connection-mode** socket
2. Address: points to a *sockaddr* structure containing the peer address.
3. address_len: specifies the length of the `sockaddr` structure

*Return value:*

Upon successful completion, returns 0, otherwise -1 is returned and *errno* is set to indicate the error.

*Notes:*

- If the connection cannot be established immediately and `O_NONBLOCK` is not set for the file descriptor for the socket, `connect()` will **block** for up to an unspecified timeout interval until the connection is established. If the timeout interval expires before the connection is established, `connect()` will **fail** and the connection attempt will be **aborted**.
- If `connect()` is interrupted by a signal that is caught while blocked, `connect()` will **fail** and set *errno* to [EINTR], the connection request will not be aborted, but established **asynchronously**.
- If the connection cannot be established immediately and `O_NONBLOCK` is set, `connect()` will **fail** and set errno to [EINPROGRESS], but the connection request will be established **asynchronously**.

> When the connection has been established asynchronously, `select()` and `poll()` will indicate that the file descriptor for the socket is ready for writing.



##### accept - accept a new connection on a socket

```c++
int accept (int socket, struct sockaddr *address, socklen_t *address_len);
```

*Arguments*:

1. socket: a socket created by `socket()`, bound to an address by `bind()`, and has called `listen()`
2. address: a null pointer, or a pointer to a `sockaddr` that holds the returned client address
3. address_len: points to a `socklen_t` type, which on input specifies the length of the supplied sockaddr structure, and on output specifies the length of the stored address.

*Return value:*

Upon successful completion, returns the nonnegative file descriptor of the accepted socket, otherwise -1.

*Note:*

- If address is not null, the address of the peer for the accepted connection is stored in the `sockaddr` structure pointed to by address, and the length of this address is stored in the object pointed to by address_len.
- If the actual length of the address is greater than the length of the supplied sockaddr structure, the stored address will be truncated.
- If the listen queue is empty and `O_NONBLOCK` is not set on the socket file descriptor, `accept()` will **block** until a connection is present. If the queue is empty and `O_NONBLOCK` is set, accept() will **fail** and set errno to [EAGAIN] or [EWOULDBLOCK].




##### read/write

> `read`/`write` are for TCP, for UDP, use `sendto`/`recvfrom`

> Linux系统的文件描述符理论上是有限的，在使用完一个fd之后，需要使用头文件`<unistd.h>`中的`close`函数关闭。更多内核相关知识可以参考Robert Love《Linux内核设计与实现》的第三版。



##### block vs non-block

- Block: block until data arrive. 
- Non-block: returns immediately. if no data, write EWOULDBLOCK or EAGAIN to `errno`






### epoll -  \<sys/epoll.h\>

> not available on MacOS

##### select & poll

`select` and `poll` are available on every Unix system, but `epoll` is Linux specific.  `poll` 基于轮询机制，效率较低： 仅知道有I/O事件发生，却不知哪几个流，只会无差异轮询所有流，找出能读/写数据的流进行操作。同时处理的流越多，无差别轮询时间越长 - O(n)。`poll` 和 `select` 类似，只是描述fd集合的方式不同，使用 `pollfd` 结构而非 `fd_set` 结构。

##### epoll

epoll模型修改主动轮询为被动通知，当有事件发生时，被动接收通知。所以epoll模型注册套接字后，主程序可做其他事情，当事件发生时，接收到通知后再去处理。

##### trigger mode

- level-triggered (LT): keep nagging you as long as the interested file descriptors are ready. Every event is guaranteed to be handled, but will have a side effect on performance.


- edge-triggered (ET): get notifications **once** a file descriptor becomes readable. Will be more concurrency friendly, but will be more difficult to program. 注意ET模式必须搭配非阻塞式socket使用，触发可读事件以后，一定要一次性把 socket 上的数据收取干净才行，也就是说一定要循环调用 recv 函数直到 recv 出错，错误码是EWOULDBLOCK（EAGAIN 一样）（此时表示 socket 上本次数据已经读完），所以接受连接最好不要用ET模式

##### usage

- `epoll_create`: opens an eventpoll file descriptor, return -1 on failure

  ```c++
  int epfd = epoll_create(1024);  // 参数表示监听事件的大小，如超过内核会自动调整，已经被舍弃，无实际意义，传入一个大于0的数即可
  int epfd = epoll_create1(0);    //参数是一个flag，一般设为0，详细参考man epoll
  ```

- `epoll_ctl`: tell the kernel file descriptors you’re interested in updates about. Interestingly, you can give it lots of different kinds of file descriptors (pipes, FIFOs, sockets, POSIX message queues, inotify instances, devices, & more), but **NOT regular files**. 

- `epoll_wait`: wait for updates about the list of files you’re interested in.


##### epoll event

`epoll_event` struct has a union type member, `data`. This could be used to store a pointer to user defined structure.

```c++
typedef union epoll_data {
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;
struct epoll_event {
  uint32_t events;	/* Epoll events */
  epoll_data_t data;	/* User data variable */
} __EPOLL_PACKED;
```





### Error Handling

 在《Effective C++》中条款08讲到，别让异常逃离析构函数。在这里我拓展一下，我们不应该放过每一个异常，否则在大型项目开发中一定会遇到很难定位的bug！对于Linux系统调用，常见的错误提示方式是使用返回值和设置errno来说明错误类型。通常来讲，当一个系统调用返回-1，说明有error发生。

```c++
void errif(bool condition, const char *errmsg){
    if(condition){
      	// perror(errmsg); 					// c style, defined in <stdio.h>
      	std::cerr << errmsg << std::endl; 	// c++ style
        exit(EXIT_FAILURE);	// terminates the process, return a failure status to host
    }
}
```





### Event Driven

- Reactor 是非阻塞同步网络模式，感知的是就绪可读写事件。在每次感知到有事件发生（比如可读就绪事件）后，就需要应用进程主动调用 read 方法来完成数据的读取，也就是要应用进程主动将 socket 接收缓存中的数据读到应用进程内存中，这个过程是同步的，读取完数据后应用进程才能处理数据。

- Proactor 是异步网络模式， 感知的是已完成的读写事件。在发起异步读写请求时，需要传入数据缓冲区的地址（用来存放结果数据）等信息，这样系统内核才可以自动帮我们把数据的读写工作完成，这里的读写工作全程由操作系统来做，并不需要像 Reactor 那样还需要应用进程主动发起 read/write 来读写数据，操作系统完成读写工作后，就会通知应用进程直接处理数据。

  可惜的是，在 Linux 下的异步 I/O 是不完善的， `aio` 系列函数是由 POSIX 定义的异步操作接口，不是真正的操作系统级别支持的，而是在用户空间模拟出来的异步，并且仅仅支持基于本地文件的 aio 异步操作，[网络编程](https://www.zhihu.com/search?q=%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra=%7B%22sourceType%22%3A%22answer%22%2C%22sourceId%22%3A1856426252%7D)中的 socket 是不支持的，这也使得基于 Linux 的高性能[网络程序](https://www.zhihu.com/search?q=%E7%BD%91%E7%BB%9C%E7%A8%8B%E5%BA%8F&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra=%7B%22sourceType%22%3A%22answer%22%2C%22sourceId%22%3A1856426252%7D)都是使用 Reactor 方案。



### QUIC

