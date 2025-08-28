#include "logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

void Logger::log(LogLevel level, const std::string& message) {
    const char* level_str = nullptr;
    switch (level) {
        case LogLevel::INFO:  level_str = "INFO"; break;
        case LogLevel::WARN:  level_str = "WARN"; break;
        case LogLevel::ERROR: level_str = "ERROR"; break;
    }

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string time_str = std::ctime(&now);
    time_str.pop_back(); // remove trailing \n

    std::cout << "[" << time_str << "] [" << level_str << "] " << message << std::endl;
}
