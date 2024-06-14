#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <memory>

void handle_read(Channel *channel);

int main() {
    auto serv_addr = std::make_unique<InetAddress>("127.0.0.1", 1145);
    auto serv_socket = std::make_unique<Socket>();
    serv_socket->bind(serv_addr.get());
    serv_socket->listen();
    serv_socket->setnonblocking();

    auto epoll = std::make_unique<Epoll>();
    auto serv_channel = std::make_unique<Channel>(epoll.get(), serv_socket->fd());
    serv_channel->enable_read();

    while (true) {
        int n = epoll->wait();
        for (int i = 0; i < n; ++i) {
            struct epoll_event &ev = epoll->get(i);
            if (ev.data.fd == serv_socket->fd()) {
                int clntfd = serv_socket->accept(serv_addr.get());
                std::cout << "accept a new connection: " << clntfd << std::endl;
                epoll->add(clntfd, EPOLLIN);
                // auto clnt_addr = std::make_unique<InetAddress>();
                // auto clnt_socket = std::make_unique<Socket>(serv_socket->accept(clnt_addr.get()));
                // std::cout << "accept a new connection: " << clnt_socket->fd() << std::endl;
                // clnt_socket->setnonblocking();
                // auto clnt_channel = std::make_unique<Channel>(epoll.get(), clnt_socket->fd());
                // clnt_channel->enable_read();

            } else {
                char buf[1024];
                int nread = read(ev.data.fd, buf, sizeof(buf));
                if (nread == 0) {
                    std::cout << "client " << ev.data.fd << " close the connection" << std::endl;
                    epoll->del(ev.data.fd);
                    close(ev.data.fd);
                } else if (nread == -1) {
                    std::cout << "read error" << std::endl;
                    epoll->del(ev.data.fd);
                    close(ev.data.fd);
                } else {
                    buf[nread] = '\0';
                    std::cout << "read " << nread << " bytes: " << buf << " from client " << ev.data.fd << std::endl;
                    write(ev.data.fd, buf, nread);
                }
            }
        }
    }
}

void handle_read(Channel *channel) {
    char buf[1024];
    int nread = read(channel->fd(), buf, sizeof(buf));
    if (nread == 0) {
        std::cout << "client close the connection" << std::endl;
        channel->disable_all();
    } else if (nread == -1) {
        std::cout << "read error" << std::endl;
        channel->disable_all();
    } else {
        buf[nread] = '\0';
        std::cout << "read " << nread << " bytes: " << buf << std::endl;
        write(channel->fd(), buf, nread);
    }
}