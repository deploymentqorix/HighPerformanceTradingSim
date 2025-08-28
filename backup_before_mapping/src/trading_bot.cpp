#include "trading_bot.h"
#include "logger.h"

TradingBot::TradingBot(int bot_id, const std::string& script_path)
    : id_(bot_id), script_path_(script_path) {}

void TradingBot::submitOrder(const Order& order) {
    Logger::log(LogLevel::INFO, "Bot " + std::to_string(id_) + ": Submitting order (Stub).");
}
void TradingBot::cancelOrder(int orderId) {
    Logger::log(LogLevel::INFO, "Bot " + std::to_string(id_) + ": Cancelling order (Stub).");
}
MarketDepth TradingBot::getMarketData(const std::string& instrument) {
    Logger::log(LogLevel::INFO, "Bot " + std::to_string(id_) + ": Fetching market data (Stub).");
    return MarketDepth{};
}
std::optional<Position> TradingBot::getPosition(const std::string& instrument) {
    Logger::log(LogLevel::INFO, "Bot " + std::to_string(id_) + ": Fetching position (Stub).");
    return std::nullopt;
}
void TradingBot::onTrade(const Trade& trade) {
    Logger::log(LogLevel::INFO, "Bot " + std::to_string(id_) + ": Event onTrade received (Stub).");
}
void TradingBot::onBookUpdate(const MarketDepth& depth) {
    Logger::log(LogLevel::INFO, "Bot " + std::to_string(id_) + ": Event onBookUpdate received (Stub).");
}