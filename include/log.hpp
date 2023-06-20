#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>

class Logger {
public:
    static Logger& get_instance();
    void log_to_file(const std::string &message);
    void stop_logging();

private:
    Logger();
    ~Logger();
    void start_logging_thread();
    void write_to_file(const std::string &message);

    std::mutex queue_mutex;
    std::queue<std::string> log_queue;
    std::condition_variable cond_var;
    bool is_logging;
    bool stop_flag;
    std::thread logging_thread;
};
