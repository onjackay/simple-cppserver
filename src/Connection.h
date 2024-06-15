#pragma once

#include <functional>

constexpr int READ_BUFFER = 1024;

class EventLoop;
class Socket;
class Channel;

class Connection {
private:
    EventLoop *loop_;
    Socket *socket_;
    Channel *channel_;
    std::function<void(Socket *)> deleteConnectionCallback_;

public:
    Connection(EventLoop *loop, Socket *socket);
    ~Connection();

    void echo(Socket *socket);
    void setDeleteConnectionCallback(const std::function<void(Socket *)> &cb);
};