#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <memory>

constexpr int MAX_EVENTS = 1024;
constexpr int BUF_SIZE = 1024;

void handle_read(Channel *channel);

int main() {
    // auto serv_addr = std::make_unique<InetAddress>("127.0.0.1", 1145);
    auto serv_addr = new InetAddress("127.0.0.1", 1145);
    // auto serv_socket = std::make_unique<Socket>();
    auto serv_socket = new Socket();
    // serv_socket->bind(serv_addr.get());
    serv_socket->bind(serv_addr);
    serv_socket->listen();

    // auto epoll = std::make_unique<Epoll>();
    auto epoll = new Epoll();
    serv_socket->setnonblocking();
    // auto serv_channel = std::make_unique<Channel>(epoll.get(), serv_socket->fd());
    auto serv_channel = new Channel(epoll, serv_socket->fd());
    serv_channel->enable_read();

    while (true) {
        std::cout << "waiting for events" << std::endl;
        int n = epoll->wait();
        for (int i = 0; i < n; ++i) {
            Channel *channel = epoll->get(i);
            std::cout << "event from fd " << channel->fd() << std::endl;
            if (channel->fd() == serv_socket->fd()) {
                // auto clnt_addr = std::make_unique<InetAddress>();
                auto clnt_addr = new InetAddress();
                // auto clnt_socket = std::make_unique<Socket>(serv_socket->accept(clnt_addr.get()));
                auto clnt_socket = new Socket(serv_socket->accept(clnt_addr));
                std::cout << "new client fd: " << clnt_socket->fd() << 
                    " from " << clnt_addr->ip() << ":" << clnt_addr->port() << std::endl;    
                clnt_socket->setnonblocking();
                // auto clnt_channel = std::make_unique<Channel>(epoll.get(), clnt_socket->fd());
                auto clnt_channel = new Channel(epoll, clnt_socket->fd());
                clnt_channel->enable_read();
            } 
            else if (channel->revents() & EPOLLIN) {
                handle_read(channel);
            }
            else {
                std::cout << "unexpected event" << std::endl;
            }
        }
    }
    delete serv_addr;
    delete serv_socket;
    delete epoll;
    delete serv_channel;
}

void handle_read(Channel *channel) {
    std::cout << "handle read" << std::endl;
    char buf[BUF_SIZE];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(channel->fd(), buf, sizeof(buf));
        if (read_bytes == -1) {
            if (errno == EINTR) { // interrupted by signal
                std::cout << "read interrupted" << std::endl;
                continue;
            }
            else if (errno == EAGAIN) { // no data to read
                std::cout << "read later" << std::endl;
                break;
            }
            else { // error
                std::cout << "read error" << std::endl;
                break;
            }
        }
        else if (read_bytes == 0) { // client disconnected
            std::cout << "client disconnected" << std::endl;
            close(channel->fd());
            break;
        }
        else {
            std::cout << "message from client " << channel->fd() << ": " << buf << std::endl;
            write(channel->fd(), buf, read_bytes);
        }
    }
}