#include "Server.h"

#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>

#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"

constexpr int READ_BUFFER = 1024;

Server::Server(EventLoop *loop, int port) : loop_(loop) {
    acceptor_ = new Acceptor(loop_, port);
    acceptor_->setNewConnectionCallback(
        std::bind(&Server::newConnection, this, std::placeholders::_1));
}

Server::~Server() { delete acceptor_; }

void Server::handleReadEvent(int sockfd) {
    char buf[READ_BUFFER];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading");
            continue;
        } else if (bytes_read == -1 &&
                   ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if (bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}

void Server::newConnection(Socket *socket) {
    InetAddress *addr = new InetAddress();
    Socket *clnt_socket = new Socket(socket->accept(addr));
    clnt_socket->setNonBlocking();
    std::cout << "new connection " << clnt_socket->fd() << " from "
              << inet_ntoa(addr->addr_.sin_addr) << ":"
              << ntohs(addr->addr_.sin_port) << std::endl;
    Channel *clnt_channel = new Channel(loop_, clnt_socket->fd());
    auto cb = std::bind(&Server::handleReadEvent, this, clnt_socket->fd());
    clnt_channel->setCallback(cb);
    clnt_channel->enableRead();
}