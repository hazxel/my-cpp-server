#ifndef MY_CPP_SERVER_THREADPOOL_HPP
#define MY_CPP_SERVER_THREADPOOL_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>

class ThreadPool {
private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mtx_;
    std::condition_variable tasks_cv_;
    bool is_running_;
public:
    ThreadPool(int size = std::thread::hardware_concurrency());  // default size is usually the number of cores
    ~ThreadPool();
    void add_task(const std::function<void()> &);
};

#endif //MY_CPP_SERVER_THREADPOOL_HPP