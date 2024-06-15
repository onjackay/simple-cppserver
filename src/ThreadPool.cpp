#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this] {
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->tasks_mtx_);
                    this->cv_.wait(lock, [this] {
                        return this->stop_ || !this->tasks_.empty();
                    });
                    if (this->stop_ && this->tasks_.empty()) {
                        return;
                    }
                    task = std::move(this->tasks_.front());
                    this->tasks_.pop();
                }
                task();
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

void ThreadPool::addTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx_);
        if (stop_) {
            throw std::runtime_error("Cannot add task to stop_ped ThreadPool");
        }
        tasks_.emplace(task);
    }
    cv_.notify_one();
}