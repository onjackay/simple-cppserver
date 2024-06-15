#include "Connection.h"

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <functional>
#include <arpa/inet.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.h"

Connection::Connection(EventLoop *loop, Socket *socket)
    : loop_(loop), socket_(socket) {
    channel_ = new Channel(loop_, socket_->fd());
    auto cb = std::bind(&Connection::echo, this, socket->fd());
    channel_->setCallback(cb);
    channel_->enableRead();
}

Connection::~Connection() {
    delete channel_;
    delete socket_;
}

void Connection::acceptConnection() {
    InetAddress *addr = new InetAddress();
    Socket *clnt_socket = new Socket(socket_->accept(addr));
    clnt_socket->setNonBlocking();
    std::cout << "new connection " << clnt_socket->fd() << " from "
              << inet_ntoa(addr->addr_.sin_addr) << ":"
              << ntohs(addr->addr_.sin_port) << std::endl;
    auto conn = new Connection(loop_, clnt_socket);
    conn->setDeleteConnectionCallback(deleteConnectionCallback_);
}

void Connection::echo(int sockfd) {
    char buf[READ_BUFFER];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading");
            continue;
        } else if (bytes_read == -1 &&
                   ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if (bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", sockfd);
            deleteConnectionCallback_(socket_);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(
    const std::function<void(Socket *)> &cb) {
    deleteConnectionCallback_ = cb;
}