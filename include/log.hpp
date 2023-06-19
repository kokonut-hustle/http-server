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
    void writeToFile(const std::string &message);

    std::mutex queueMutex;
    std::queue<std::string> logQueue;
    std::condition_variable conditionVariable;
    bool isLogging;
    bool stopFlag;
    std::thread loggingThread;
};
