#pragma once

#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection {
private:
    EventLoop *loop_;
    Socket *sock_;
    Channel *channel_;
    std::function<void(Socket *)> deleteConnectionCallback_;
    Buffer *read_buf_;

public:
    Connection(EventLoop *loop, Socket *sock);
    ~Connection();

    void echo(Socket *sock);
    void setDeleteConnectionCallback(const std::function<void(Socket *)> &cb);
};