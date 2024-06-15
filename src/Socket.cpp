#include "Socket.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "InetAddress.h"
#include "util.h"

Socket::Socket() : fd_(socket(AF_INET, SOCK_STREAM, 0)) {
    errif(fd_ == -1, "socket create error");
}

Socket::Socket(int fd) : fd_(fd) {
    errif(fd_ == -1, "socket create error");
}

Socket::~Socket() {
    if (fd_ != -1) {
        close(fd_);
    }
}

void Socket::bind(InetAddress *addr) {
    errif(::bind(fd_, addr->addr(), *addr->addrLen()) == -1,
          "socket bind error");
}

void Socket::listen() {
    errif(::listen(fd_, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setNonBlocking() {
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr) {
    int clntfd = ::accept(fd_, addr->addr(), addr->addrLen());
    errif(clntfd == -1, "socket accept error");
    return clntfd;
}

void Socket::connect(InetAddress *addr) {
    errif(::connect(fd_, addr->addr(), *addr->addrLen()) == -1,
          "socket connect error");
}