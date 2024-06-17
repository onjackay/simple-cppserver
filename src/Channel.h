#pragma once
#include <sys/epoll.h>
#include <functional>

class Epoll;
class EventLoop;

class Channel {
private:
    EventLoop *loop_;
    int fd_;
    uint32_t events_;
    uint32_t revents_;
    bool in_epoll_;
    bool use_thread_pool_;
    std::function<void()> callback_;

public:
    Channel(EventLoop *loop, int fd); 
    ~Channel();

    void enableRead();
    void enableWrite();
    void enableET();
    void disableAll();
    void handleEvent();

    void setRevents(uint32_t revents) { revents_ = revents; }
    void setInEpoll(bool in_epoll) { in_epoll_ = in_epoll; }
    void setCallback(const std::function<void()> &callback) { callback_ = callback; }
    void setUseThreadPool(bool use_thread_pool) { use_thread_pool_ = use_thread_pool; }

    int fd() const { return fd_; }
    uint32_t events() const { return events_; }
    uint32_t revents() const { return revents_; }
    bool inEpoll() const { return in_epoll_; }
};