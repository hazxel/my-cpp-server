#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>

#include "my-cpp-server/threadpool/threadpool.hpp"

ThreadPool::ThreadPool(int size) : is_running_(true) {
    for (int i = 0; i < size; i++) {
        threads_.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx_);
                    tasks_cv_.wait(lock, [this] { return !tasks_.empty() || !is_running_; });
                    if (!is_running_ && tasks_.empty()) {
                        return;
                    }
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx_);
        is_running_ = false;
    }
    tasks_cv_.notify_all();
    for (auto &thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        } else {
            thread.detach();
        }
    }
}

void ThreadPool::add_task(const std::function<void()> &task) {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx_);
        tasks_.emplace(task);
    }
    tasks_cv_.notify_one();
}