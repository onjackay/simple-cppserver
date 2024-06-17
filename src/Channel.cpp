#include "Channel.h"

#include "Epoll.h"
#include "EventLoop.h"
#include <iostream>

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(0),
      revents_(0),
      in_epoll_(false),
      use_thread_pool_(true) {}

Channel::~Channel() {}

void Channel::enableRead() {
    events_ |= EPOLLIN;
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
    if (revents_ & EPOLLIN) {
        if (read_cb_ == nullptr) {
            std::cerr << "read callback is null" << std::endl;
            return;
        }
        if (use_thread_pool_) {
            loop_->addTask(read_cb_);
        } else {
            read_cb_();
        }
    }
    if (revents_ & EPOLLOUT) {
        if (write_cb_ == nullptr) {
            std::cerr << "write callback is null" << std::endl;
            return;
        }
        if (use_thread_pool_) {
            loop_->addTask(write_cb_);
        } else {
            write_cb_();
        }
    }
}