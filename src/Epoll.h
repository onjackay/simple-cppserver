#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll {
private:
    int fd_;
    // std::vector<struct epoll_event> events_;
    struct epoll_event *events_;

public:
    Epoll();
    ~Epoll();

    // void add(int fd, uint32_t events);
    // void mod(int fd, uint32_t events);
    // void del(int fd);
    std::vector<Channel *> poll(int timeout = -1);

    void updateChannel(Channel *channel);

    int fd() const { return fd_; }
};