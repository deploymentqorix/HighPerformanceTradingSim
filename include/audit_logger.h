#pragma once
#include "command.h"
#include <fstream>
#include <string>

class AuditLogger {
public:
    explicit AuditLogger(const std::string& filepath);
    ~AuditLogger();
    void logCommand(const Command& command);

private:
    std::ofstream audit_file_;
};
