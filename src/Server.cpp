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

void Server::newConnection(Socket *sock) {
    auto conn = new Connection(loop_, sock);
    conn->setDeleteConnectionCallback(
        std::bind(&Server::deleteConnection, this, std::placeholders::_1));
    connections_[sock->fd()] = conn;
}

void Server::deleteConnection(Socket *sock) {
    delete connections_[sock->fd()];
    connections_.erase(sock->fd());
}