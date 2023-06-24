#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>

#define Log(x) Logger::get_instance().log(x)
#define LogInfo(x) Logger::get_instance().log_info(x)
#define LogErr(x) Logger::get_instance().log_error(x)
#define LogWarn(x) Logger::get_instance().log_warn(x)

class Logger {
public:
    static Logger& get_instance();
    void log(const std::string &message);
    void log_info(const std::string &message);
    void log_error(const std::string &message);
    void log_warn(const std::string &message);

private:
    Logger();
    ~Logger();
    void stop_logging();
    void start_logging_thread();
    void write_to_file(const std::string &message);

    std::mutex queue_mutex;
    std::queue<std::string> log_queue;
    std::condition_variable cond_var;
    bool is_logging;
    bool stop_flag;
    std::thread logging_thread;
};
