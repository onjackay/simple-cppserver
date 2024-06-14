#include "Epoll.h"
#include "Channel.h"
#include "util.h"
#include <unistd.h>
#include <cstring>

constexpr int MAX_EVENTS = 1024;

Epoll::Epoll() : epollfd_(epoll_create1(0)), events_(MAX_EVENTS) {
    errif(epollfd_ == -1, "epoll create error");
}

Epoll::~Epoll() {
    if (epollfd_ != -1) {
        close(epollfd_);
    }
}

void Epoll::add(int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    errif(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
}

void Epoll::mod(int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    errif(epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll mod error");
}

void Epoll::del(int fd) {
    errif(epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, nullptr) == -1, "epoll del error");
}

int Epoll::wait() {
    int n = epoll_wait(epollfd_, events_.data(), MAX_EVENTS, -1);
    errif(n == -1, "epoll wait error");
    return n;
}

void Epoll::update_channel(Channel *channel) {
    if (channel->in_epoll()) {
        mod(channel->fd(), channel->events());
    } else {
        add(channel->fd(), channel->events());
        channel->set_in_epoll(true);
    }
}

struct epoll_event &Epoll::get(int i) {
    return events_[i];
}