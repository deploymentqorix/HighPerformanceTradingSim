#include "scripting_engine.h"
#include "logger.h"

ScriptingEngine::ScriptingEngine() {}

void ScriptingEngine::loadBotsFromDirectory(const std::string& dir_path) {
    Logger::log(LogLevel::INFO, "ScriptingEngine: Loading bot stubs from " + dir_path);
    bots_.emplace_back(1, "scripts/my_strategy.lua");
    Logger::log(LogLevel::INFO, "ScriptingEngine: Created dummy bot stub with ID 1.");
}