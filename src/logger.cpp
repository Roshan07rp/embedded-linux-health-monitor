#include "logger.h"
#include <fstream>
#include <iostream>
#include <ctime>

Logger::Logger(const std::string& file_path) : file_path_(file_path) {}

void Logger::log(LogLevel level, const std::string& message) {
    const char* label = "INFO";
    if (level == LogLevel::WARNING) label = "WARNING";
    if (level == LogLevel::ERROR) label = "ERROR";

    std::time_t now = std::time(nullptr);
    std::tm tm_now{};
    localtime_r(&now, &tm_now);

    char timestamp[32];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_now);

    const std::string line =
        std::string("[") + timestamp + "] [" + label + "] " + message;

    std::cout << line << '\n';

    if (!file_path_.empty()) {
        std::ofstream file(file_path_, std::ios::app);
        if (file) file << line << '\n';
    }
}
