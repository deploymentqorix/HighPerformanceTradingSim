#pragma once
#include "trading_bot.h"
#include <vector>
#include <string>

class ScriptingEngine {
public:
    ScriptingEngine();
    void loadBotsFromDirectory(const std::string& dir_path);

private:
    std::vector<TradingBot> bots_;
};
