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
    auto conn = new Connection(loop_, socket);
    conn->setDeleteConnectionCallback(
        std::bind(&Server::deleteConnection, this, std::placeholders::_1));
    connections_[socket->fd()] = conn;
}

void Server::deleteConnection(Socket *socket) {
    delete connections_[socket->fd()];
    connections_.erase(socket->fd());
}