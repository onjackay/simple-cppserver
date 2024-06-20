#include "Channel.h"

#include <unistd.h>

#include <iostream>

#include "Epoll.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), in_epoll_(false),
      use_thread_pool_(true) {}

Channel::~Channel() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void Channel::enableRead() {
  events_ |= EPOLLIN | EPOLLPRI;
  loop_->updateChannel(this);
}

void Channel::enableWrite() {
  events_ |= EPOLLOUT;
  loop_->updateChannel(this);
}

void Channel::enableET() {
  events_ |= EPOLLET;
  loop_->updateChannel(this);
}

void Channel::disableAll() {
  events_ = 0;
  loop_->updateChannel(this);
}

void Channel::handleEvent() {
  if (revents_ & (EPOLLIN | EPOLLPRI)) {
    if (read_cb_ == nullptr) {
      std::cerr << "read callback is null" << std::endl;
      return;
    }
    read_cb_();
  }
  if (revents_ & EPOLLOUT) {
    if (write_cb_ == nullptr) {
      std::cerr << "write callback is null" << std::endl;
      return;
    }
    write_cb_();
  }
}