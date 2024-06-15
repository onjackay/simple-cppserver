#include "Acceptor.h"

#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Server.h"
#include "Socket.h"

#include <iostream>

Acceptor::Acceptor(EventLoop *loop, int port) : loop_(loop) {
    sock_ = new Socket();
    auto addr = new InetAddress("127.0.0.1", port);
    sock_->bind(addr);
    sock_->listen();
    sock_->setNonBlocking();
    channel_ = new Channel(loop_, sock_->fd());
    auto cb = std::bind(&Acceptor::acceptConnection, this);
    channel_->setCallback(cb);
    channel_->enableRead();
    delete addr;
}

Acceptor::~Acceptor() {
    delete sock_;
    delete channel_;
}

void Acceptor::acceptConnection() { 
    InetAddress *addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock_->accept(addr));
    clnt_sock->setNonBlocking();
    std::cout << "new connection " << clnt_sock->fd() << " from "
              << inet_ntoa(addr->addr_.sin_addr) << ":"
              << ntohs(addr->addr_.sin_port) << std::endl;

    newConnectionCallback_(clnt_sock); 
    delete addr;
}

void Acceptor::setNewConnectionCallback(
    const std::function<void(Socket *)> &cb) {
    newConnectionCallback_ = cb;
}