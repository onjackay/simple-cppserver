#include "Epoll.h"
#include "Channel.h"
#include "util.h"
#include <unistd.h>
#include <cstring>
#include <iostream>

constexpr int MAX_EVENTS = 1024;

Epoll::Epoll() : epollfd_(epoll_create1(0)) {
    errif(epollfd_ == -1, "epoll create error");
    events_ = new struct epoll_event[MAX_EVENTS];
    bzero(events_, sizeof(struct epoll_event) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epollfd_ != -1) {
        close(epollfd_);
    }
    delete[] events_;
}

void Epoll::add(int fd, uint32_t events) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = events;
    ev.data.fd = fd;
    errif(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
}

void Epoll::mod(int fd, uint32_t events) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = events;
    ev.data.fd = fd;
    errif(epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll mod error");
}

void Epoll::del(int fd) {
    errif(epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, nullptr) == -1, "epoll del error");
}

int Epoll::wait() {
    int n = epoll_wait(epollfd_, events_, MAX_EVENTS, -1);
    errif(n == -1, "epoll wait error");
    return n;
}

void Epoll::update_channel(Channel *channel) {
    int fd = channel->fd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->events();
    if (channel->in_epoll()) {
        errif(epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll mod error");
    } else {
        errif(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->set_in_epoll(true);
    }
}

std::vector<Channel *> Epoll::poll(int timeout) {
    int n = epoll_wait(epollfd_, events_, MAX_EVENTS, timeout);
    errif(n == -1, "epoll wait error");
    std::vector<Channel *> channels;
    for (int i = 0; i < n; ++i) {
        auto channel = static_cast<Channel *>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        channels.push_back(channel);
    }
    return channels;
}

Channel *Epoll::get(int i) {
    auto channel = static_cast<Channel *>(events_[i].data.ptr);
    channel->set_revents(events_[i].events);
    return channel;
}