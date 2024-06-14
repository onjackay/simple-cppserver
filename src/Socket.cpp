#include "Socket.h"
#include "util.h"
#include "InetAddress.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>


Socket::Socket() : sockfd_(socket(AF_INET, SOCK_STREAM, 0)) {
    errif(sockfd_ == -1, "socket create error");
}

Socket::Socket(int sockfd) : sockfd_(sockfd) {
    errif(sockfd_ == -1, "socket create error");
}

Socket::~Socket() {
    if (sockfd_ != -1) {
        close(sockfd_);
    }
}

void Socket::bind(InetAddress *addr) {
    errif(::bind(sockfd_, addr->addr(), *addr->addr_len()) == -1, "socket bind error");
}

void Socket::listen() {
    errif(::listen(sockfd_, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking() {
    fcntl(sockfd_, F_SETFL, fcntl(sockfd_, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr) {
    int clntfd = ::accept(sockfd_, addr->addr(), addr->addr_len());
    return clntfd;
}
