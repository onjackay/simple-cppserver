#include "Server.h"

#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <thread>

#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"

Server::Server(EventLoop *loop, int port, int thread_cnt)
    : main_reactor_(loop) {
    acceptor_ = new Acceptor(main_reactor_, port);
    acceptor_->setNewConnectionCallback(
        std::bind(&Server::newConnection, this, std::placeholders::_1));

    for (int i = 0; i < thread_cnt; i++) {
        sub_reactors_.push_back(new EventLoop);
        threads_.push_back(std::thread(&EventLoop::loop, sub_reactors_[i]));
    }
}

Server::~Server() {
    delete acceptor_;
    for (auto reactor : sub_reactors_) {
        delete reactor;
    }
    for (auto conn : connections_) {
        delete conn.second;
    }
}

void Server::newConnection(Socket *sock) {
    if (sock->fd() == -1) return;
    int idx = sock->fd() % sub_reactors_.size();
    Connection *conn = new Connection(sub_reactors_[idx], sock);
    conn->setDeleteConnectionCallback(
        std::bind(&Server::deleteConnection, this, std::placeholders::_1));
    connections_[sock->fd()] = conn;
}

void Server::deleteConnection(Socket *sock) {
    if (sock->fd() == -1) return;
    auto it = connections_.find(sock->fd());
    if (it != connections_.end()) {
        auto conn = it->second;
        connections_.erase(it);
        delete conn;
    }
}