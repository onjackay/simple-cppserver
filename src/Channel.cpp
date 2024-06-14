#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll *epoll, int fd)
    : epoll_(epoll), fd_(fd), events_(0), revents_(0), in_epoll_(false) {}

Channel::~Channel() {}

void Channel::enable_read() {
    events_ = EPOLLIN | EPOLLET;
    epoll_->update_channel(this);
}

void Channel::enable_write() {
    events_ = EPOLLOUT | EPOLLET;
    epoll_->update_channel(this);
}

void Channel::disable_all() {
    events_ = 0;
    epoll_->update_channel(this);
}
