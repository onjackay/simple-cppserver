#include "Acceptor.h"

#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Server.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop *loop, int port) : loop_(loop) {
    socket_ = new Socket();
    addr_ = new InetAddress("127.0.0.1", port);
    socket_->bind(addr_);
    socket_->listen();
    socket_->setNonBlocking();
    channel_ = new Channel(loop_, socket_->fd());
    auto cb = std::bind(&Acceptor::acceptConnection, this);
    channel_->setCallback(cb);
    channel_->enableRead();
}

Acceptor::~Acceptor() {
    delete socket_;
    delete addr_;
    delete channel_;
}

void Acceptor::acceptConnection() { newConnectionCallback_(socket_); }

void Acceptor::setNewConnectionCallback(
    const std::function<void(Socket *)> &cb) {
    newConnectionCallback_ = cb;
}