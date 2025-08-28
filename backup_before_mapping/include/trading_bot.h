#pragma once
#include "order.h"
#include "position.h"
#include "market_data.h"
#include "trade.h"
#include <string>
#include <optional>

class TradingBot {
public:
    explicit TradingBot(int bot_id, const std::string& script_path);
    void submitOrder(const Order& order);
    void cancelOrder(int orderId);
    MarketDepth getMarketData(const std::string& instrument);
    std::optional<Position> getPosition(const std::string& instrument);
    void onTrade(const Trade& trade);
    void onBookUpdate(const MarketDepth& depth);

private:
    int id_;
    std::string script_path_;
};
