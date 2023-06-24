#include <chrono>
#include <iomanip>

#include "log.hpp"

Logger::Logger() : stop_flag(false), is_logging(false) {}

Logger::~Logger() {
    stop_logging();
}

Logger& Logger::get_instance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string &message) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        // Get the current time
        auto current_time = std::chrono::system_clock::now();
        // Convert the current time to a time_t object
        std::time_t time = std::chrono::system_clock::to_time_t(current_time);
        // Convert the time point to a time string
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S ");
        std::string time_string = oss.str();

        log_queue.push(time_string + message);
    }
    cond_var.notify_one(); // Notify waiting thread

    // Start the logging thread if it's not running
    if (!is_logging) {
        start_logging_thread();
    }
}

void Logger::log_info(const std::string &message) {
    log("Info: " + message);
}

void Logger::log_error(const std::string &message) {
    log("Error: " + message);
}

void Logger::log_warn(const std::string &message) {
    log("Warn: " + message);
}

void Logger::stop_logging() {
    // Set the stop flag and notify the logging thread
    stop_flag = true;
    cond_var.notify_one();

    // Wait for the logging thread to finish
    if (logging_thread.joinable()) {
        logging_thread.join();
    }
}

void Logger::start_logging_thread() {
    is_logging = true;
    logging_thread = std::thread([this]() {
        while (true) {
            std::string message;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                cond_var.wait(lock, [this]() {
                    return !log_queue.empty() || stop_flag;
                });

                if (!log_queue.empty()) {
                    message = log_queue.front();
                    log_queue.pop();
                }
            }

            if (!message.empty()) {
                write_to_file(message);
            }

            if (stop_flag && log_queue.empty()) {
                break;
            }
        }
        is_logging = false;
    });
}

void Logger::write_to_file(const std::string &message) {
    std::ofstream log_file("log.txt", std::ios::app);
    if (log_file.is_open()) {
        log_file << message << std::endl;
        log_file.close();
    } else {
        std::cerr << "Failed to open the log file." << std::endl;
    }
}
