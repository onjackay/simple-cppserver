#include "EventLoop.h"

#include <iostream>
#include <vector>

#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"

EventLoop::EventLoop()
    : epoll_(new Epoll()), quit_(false), thread_pool_(new ThreadPool()) {}

EventLoop::~EventLoop() { delete epoll_; }

void EventLoop::loop() {
    while (!quit_) {
        std::vector<Channel*> channels = epoll_->poll();
        for (Channel* channel : channels) {
            channel->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* channel) {
    epoll_->updateChannel(channel);
}

void EventLoop::addTask(std::function<void()> func) {
    thread_pool_->addTask(func);
}