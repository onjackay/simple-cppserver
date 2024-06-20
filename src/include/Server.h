#pragma once

#include <map>
#include <vector>
#include <thread>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server {
   private:
    EventLoop *main_reactor_;  // Main reactor for accepting new connections
    std::vector<EventLoop *> sub_reactors_;  // Sub-reactors for handling connections
    Acceptor *acceptor_;
    std::map<int, Connection *> connections_;
    std::vector<std::thread> threads_;

   public:
    Server(EventLoop *loop, int port, int thread_cnt = 8);
    ~Server();

    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
};