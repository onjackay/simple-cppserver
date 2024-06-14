#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll {
private:
    int epollfd_;
    // std::vector<struct epoll_event> events_;
    struct epoll_event *events_;

public:
    Epoll();
    ~Epoll();

    void add(int fd, uint32_t events);
    void mod(int fd, uint32_t events);
    void del(int fd);
    // return the number of events
    int wait();
    std::vector<Channel *> poll(int timeout = -1);

    void update_channel(Channel *channel);
    // struct epoll_event &get(int i);
    Channel *get(int i);

    int fd() const { return epollfd_; }
};