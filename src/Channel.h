#pragma once
#include <sys/epoll.h>

class Epoll;

class Channel {
private:
    Epoll *epoll_;
    int fd_;
    uint32_t events_;
    uint32_t revents_;
    bool in_epoll_;

public:
    Channel(Epoll *epoll, int fd);
    ~Channel();

    void enable_read();
    void enable_write();
    void disable_all();

    void set_revents(uint32_t revents) { revents_ = revents; }
    void set_in_epoll(bool in_epoll) { in_epoll_ = in_epoll; }

    int fd() const { return fd_; }
    uint32_t events() const { return events_; }
    uint32_t revents() const { return revents_; }
    bool in_epoll() const { return in_epoll_; }
};