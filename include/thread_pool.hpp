#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    ThreadPool(int num_threads);
    ~ThreadPool();

    // Submit a task to the thread pool
    template <typename Function, typename... Args>
    void submit(Function&& f, Args&&... args) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.emplace([f, args...] { f(args...); });
        }

        condition.notify_one();
    }
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stop;
};
