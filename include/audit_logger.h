#include <fstream>
#include "command.h"

class AuditLogger {
public:
    explicit AuditLogger(const std::string& filename);
    void logCommand(const Command& cmd);

private:
    std::ofstream log_file;  // <-- missing field
};
