#pragma once
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor {
private:
    EventLoop *loop_;
    Socket *sock_;
    Channel *channel_;
    std::function<void(Socket *)> newConnectionCallback_;

public:
    Acceptor(EventLoop *loop, int port);
    ~Acceptor();

    void acceptConnection();
    void setNewConnectionCallback(const std::function<void(Socket *)> &cb);
};