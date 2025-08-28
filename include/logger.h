#pragma once
#include <string>
#include <sstream>

enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static void log(LogLevel level, const std::string& message);
};
