#pragma once
#include "trade_listener.h"
#include "position.h"
#include <string>
#include <unordered_map>
#include <optional>

class AccountingManager : public TradeListener {
public:
    void onTrade(const std::string& instrument, const Trade& trade) override;
    std::optional<Position> getPosition(const std::string& instrument) const;
    const std::unordered_map<std::string, Position>& getPositions() const;

private:
    std::unordered_map<std::string, Position> positions_;
};
