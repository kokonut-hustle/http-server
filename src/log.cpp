#include "log.hpp"

Logger::Logger() : stopFlag(false), isLogging(false) {}

Logger::~Logger() {
    stop_logging();
}

Logger& Logger::get_instance() {
    static Logger instance;
    return instance;
}

void Logger::log_to_file(const std::string &message) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        logQueue.push(message);
    }
    conditionVariable.notify_one(); // Notify waiting thread

    // Start the logging thread if it's not running
    if (!isLogging) {
        start_logging_thread();
    }
}

void Logger::stop_logging() {
    // Set the stop flag and notify the logging thread
    stopFlag = true;
    conditionVariable.notify_one();

    // Wait for the logging thread to finish
    if (loggingThread.joinable()) {
        loggingThread.join();
    }
}

void Logger::start_logging_thread() {
    isLogging = true;
    loggingThread = std::thread([this]() {
        while (true) {
            std::string message;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                conditionVariable.wait(lock, [this]() {
                    return !logQueue.empty() || stopFlag;
                });

                if (!logQueue.empty()) {
                    message = logQueue.front();
                    logQueue.pop();
                }
            }

            if (!message.empty()) {
                writeToFile(message);
            }

            if (stopFlag && logQueue.empty()) {
                break;
            }
        }
        isLogging = false;
    });
}

void Logger::writeToFile(const std::string &message) {
    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    } else {
        std::cerr << "Failed to open the log file." << std::endl;
    }
}
