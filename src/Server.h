#pragma once

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server {
private:
    EventLoop *loop_;
    Acceptor *acceptor_;
    // fd -> Connection *
    std::map<int, Connection *> connections_;

public:
    Server(EventLoop *loop, int port);
    ~Server();

    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
    // void handleReadEvent(Socket *sock);
};