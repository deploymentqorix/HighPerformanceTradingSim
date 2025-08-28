#pragma once
#include "matching_engine.h"
#include <string>

class AuditReplayer {
public:
    AuditReplayer(const std::string& filepath, MatchingEngine& engine);
    void run();

private:
    std::string filepath_;
    MatchingEngine& engine_;
};
