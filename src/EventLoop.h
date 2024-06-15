#pragma once

#include <functional>

class Epoll;
class Channel;
class ThreadPool;

class EventLoop {
private:
    Epoll *epoll_;
    bool quit_;
    ThreadPool *thread_pool_;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel* channel);
    void addTask(std::function<void()> func);
};