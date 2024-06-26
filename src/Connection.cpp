#include "Connection.h"

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>

#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"

Connection::Connection(EventLoop *loop, Socket *sock)
    : loop_(loop), sock_(sock) {
  channel_ = new Channel(loop_, sock_->fd());
  channel_->enableRead();
  channel_->enableET();
  auto cb = std::bind(&Connection::echo, this, sock_);
  channel_->setReadCallback(cb);
  read_buf_ = new Buffer();
}

Connection::~Connection() {
  delete channel_;
  delete sock_;
  delete read_buf_;
}

void Connection::echo(Socket *sock) {
  char buf[1024];
  while (true) {
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = read(sock->fd(), buf, sizeof(buf));
    if (bytes_read > 0) {
      read_buf_->append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) {
      printf("continue reading");
      continue;
    } else if (bytes_read == -1) {
      printf("message from client fd %d of length %d: %s\n", sock->fd(),
             read_buf_->size(), read_buf_->c_str());
      errif(write(sock->fd(), read_buf_->c_str(), read_buf_->size()) == -1,
            "write error");
      printf("write back to client fd %d of length %d: %s\n", sock->fd(),
             read_buf_->size(), read_buf_->c_str());
      read_buf_->clear();
      break;
    } else if (bytes_read == 0) {
      printf("EOF, client fd %d disconnected\n", sock->fd());
      deleteConnectionCallback_(sock);
      break;
    } else {
      printf("unexpected error\n");
      deleteConnectionCallback_(sock);
      break;
    }
  }
}

void Connection::setDeleteConnectionCallback(
    const std::function<void(Socket *)> &cb) {
  deleteConnectionCallback_ = cb;
}