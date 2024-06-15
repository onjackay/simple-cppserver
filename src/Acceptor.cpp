#include "Acceptor.h"

#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Server.h"
#include "Socket.h"

#include <iostream>

Acceptor::Acceptor(EventLoop *loop, int port) : loop_(loop) {
    socket_ = new Socket();
    auto addr = new InetAddress("127.0.0.1", port);
    socket_->bind(addr);
    socket_->listen();
    socket_->setNonBlocking();
    channel_ = new Channel(loop_, socket_->fd());
    auto cb = std::bind(&Acceptor::acceptConnection, this);
    channel_->setCallback(cb);
    channel_->enableRead();
    delete addr;
}

Acceptor::~Acceptor() {
    delete socket_;
    delete channel_;
}

void Acceptor::acceptConnection() { 
    InetAddress *addr = new InetAddress();
    Socket *clnt_socket = new Socket(socket_->accept(addr));
    clnt_socket->setNonBlocking();
    std::cout << "new connection " << clnt_socket->fd() << " from "
              << inet_ntoa(addr->addr_.sin_addr) << ":"
              << ntohs(addr->addr_.sin_port) << std::endl;

    newConnectionCallback_(clnt_socket); 
    delete addr;
}

void Acceptor::setNewConnectionCallback(
    const std::function<void(Socket *)> &cb) {
    newConnectionCallback_ = cb;
}