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

> Linux??????????????????????????????????????????????????????????????????fd??????????????????????????????`<unistd.h>`??????`close`???????????????????????????????????????????????????Robert Love???Linux???????????????????????????????????????







### Error Handling

 ??????Effective C++????????????08????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????bug?????????Linux?????????????????????????????????????????????????????????????????????errno??????????????????????????????????????????????????????????????????-1????????????error?????????

```c++
void errif(bool condition, const char *errmsg){
    if(condition){
      	// perror(errmsg); 					// c style, defined in <stdio.h>
      	std::cerr << errmsg << std::endl; 	// c++ style
        exit(EXIT_FAILURE);	// terminates the process, return a failure status to host
    }
}
```

