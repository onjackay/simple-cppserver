#pragma once

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool {
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex tasks_mtx_;
    std::condition_variable cv_;
    bool stop_;

public:
    ThreadPool(size_t num_threads = 8);
    ~ThreadPool();

    void addTask(std::function<void()> task);

};