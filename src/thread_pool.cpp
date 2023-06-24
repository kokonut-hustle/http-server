#include "thread_pool.hpp"

ThreadPool::ThreadPool(int num_threads) : stop(false) {
    // Create worker threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                // Acquire lock and check for tasks
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    condition.wait(lock, [this] {
                        return stop || !tasks.empty();
                    });

                    if (stop && tasks.empty()) {
                        return;
                    }

                    task = std::move(tasks.front());
                    tasks.pop();
                }

                // Execute the task
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    // Stop all threads
    {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }

    condition.notify_all();

    // Join all threads
    for (std::thread& thread : threads) {
        thread.join();
    }
}
