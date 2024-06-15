#pragma once

class EventLoop;
class Socket;

class Server {
private:
    EventLoop *loop_;

public:
    Server(EventLoop *loop, int port);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *socket);
};