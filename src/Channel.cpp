#include "Channel.h"

#include "Epoll.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), in_epoll_(false) {}

Channel::~Channel() {}

void Channel::enableRead() {
    events_ = EPOLLIN | EPOLLET;
    loop_->updateChannel(this);
}

void Channel::enableWrite() {
    events_ = EPOLLOUT | EPOLLET;
    loop_->updateChannel(this);
}

void Channel::disableAll() {
    events_ = 0;
    loop_->updateChannel(this);
}

void Channel::handleEvent() {
    loop_->addTask(callback_);
}