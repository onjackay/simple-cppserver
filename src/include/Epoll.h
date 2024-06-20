#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll {
private:
  int fd_;
  struct epoll_event *events_;

public:
  Epoll();
  ~Epoll();

  std::vector<Channel *> poll(int timeout = -1);

  void updateChannel(Channel *channel);
  void deleteChannel(Channel *channel);

  int fd() const { return fd_; }
};