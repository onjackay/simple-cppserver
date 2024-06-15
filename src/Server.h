#pragma once

class EventLoop;
class Socket;
class Acceptor;

class Server {
private:
    EventLoop *loop_;
    Acceptor *acceptor_;

public:
    Server(EventLoop *loop, int port);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *socket);
};