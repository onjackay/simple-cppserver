#include "ThreadPool.h"

#include <iostream>

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this] {
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx_);
                    cv_.wait(lock, [this] {
                        return stop_ || !tasks_.empty();
                    });
                    if (stop_ && tasks_.empty()) {
                        return;
                    }
                    int n = tasks_.size();
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                if (task == nullptr) {
                    std::cerr << "task is null" << std::endl;
                } else {
                    task();
                }
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx_);
        stop_ = true;
    }
    cv_.notify_all();
    for (std::thread &worker : workers_) {
        worker.join();
    }
}
