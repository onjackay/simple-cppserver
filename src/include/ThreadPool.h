#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

constexpr int DEFAULT_THREADS = 8;

class ThreadPool {
   private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex tasks_mtx_;
    std::condition_variable cv_;
    bool stop_;

   public:
    ThreadPool(size_t num_threads = DEFAULT_THREADS);
    ~ThreadPool();

    template <class F, class... Args>
    auto addTask(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;
};

template <class F, class... Args>
auto ThreadPool::addTask(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mtx_);
        if (stop_) {
            throw std::runtime_error("addTask on stopped ThreadPool");
        }
        tasks_.push([task]() { (*task)(); });
    }
    cv_.notify_one();
    return res;
}