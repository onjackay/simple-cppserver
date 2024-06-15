#include "Server.h"

#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>

#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"

Server::Server(EventLoop *loop, int port) : loop_(loop) {
    acceptor_ = new Acceptor(loop_, port);
    acceptor_->setNewConnectionCallback(
        std::bind(&Server::newConnection, this, std::placeholders::_1));
}

Server::~Server() { delete acceptor_; }

void Server::newConnection(Socket *socket) {
    InetAddress *addr = new InetAddress();
    Socket *clnt_socket = new Socket(socket->accept(addr));
    clnt_socket->setNonBlocking();
    std::cout << "new connection " << clnt_socket->fd() << " from "
              << inet_ntoa(addr->addr_.sin_addr) << ":"
              << ntohs(addr->addr_.sin_port) << std::endl;

    auto conn = new Connection(loop_, clnt_socket);
    conn->setDeleteConnectionCallback(
        std::bind(&Server::deleteConnection, this, std::placeholders::_1));
    connections_[socket->fd()] = conn;
}

void Server::deleteConnection(Socket *socket) {
    std::cout << "delete connection " << socket->fd() << std::endl;
    delete connections_[socket->fd()];
    connections_.erase(socket->fd());
    delete socket;
}

void Server::handleReadEvent(Socket *socket) {
    char buf[READ_BUFFER];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(socket->fd(), buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("message from client fd %d: %s\n", socket->fd(), buf);
            write(socket->fd(), buf, sizeof(buf));
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading");
            continue;
        } else if (bytes_read == -1 &&
                   ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if (bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", socket->fd());
            deleteConnection(socket);
            break;
        }
    }
}