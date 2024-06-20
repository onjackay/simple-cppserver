#include "Epoll.h"
#include "Channel.h"
#include "util.h"
#include <cstring>
#include <iostream>
#include <unistd.h>

constexpr int MAX_EVENTS = 1024;

Epoll::Epoll() : fd_(epoll_create1(0)) {
  errif(fd_ == -1, "epoll create error");
  events_ = new struct epoll_event[MAX_EVENTS];
  bzero(events_, sizeof(struct epoll_event) * MAX_EVENTS);
}

Epoll::~Epoll() {
  if (fd_ != -1) {
    close(fd_);
  }
  delete[] events_;
}

void Epoll::updateChannel(Channel *channel) {
  int fd = channel->fd();
  struct epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.ptr = channel;
  ev.events = channel->events();
  if (channel->inEpoll()) {
    errif(epoll_ctl(fd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll mod error");
  } else {
    errif(epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
    channel->setInEpoll(true);
  }
}

void Epoll::deleteChannel(Channel *channel) {
  int fd = channel->fd();
  errif(epoll_ctl(fd_, EPOLL_CTL_DEL, fd, nullptr) == -1, "epoll del error");
  channel->setInEpoll(false);
}

std::vector<Channel *> Epoll::poll(int timeout) {
  int n = epoll_wait(fd_, events_, MAX_EVENTS, timeout);
  errif(n == -1, "epoll wait error");
  std::vector<Channel *> channels;
  channels.reserve(n);
  for (int i = 0; i < n; ++i) {
    auto channel = reinterpret_cast<Channel *>(events_[i].data.ptr);
    channel->setRevents(events_[i].events);
    channels.push_back(channel);
  }
  return channels;
}
