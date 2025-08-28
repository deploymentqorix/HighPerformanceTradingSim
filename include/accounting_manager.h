#pragma once
#include "trade_listener.h"
#include "position.h"
#include "order.h"            // for OrderSide
#include <string>
#include <unordered_map>
#include <optional>

/*
  Mapping:
  - QCSIDM_SRS_013: Account-level P&L tracking (basic)
  - QCSIDM_SRS_014: Position tracking per symbol
  - NOTE: this is per-instrument aggregated accounting (no per-user accounts yet)
*/

class AccountingManager : public TradeListener {
public:
    // Called by other listeners (keeps compatibility)
    void onTrade(const std::string& instrument, const Trade& trade) override;

    // Called by MatchingEngine with the aggressor side (who initiated the trade)
    // aggressor == OrderSide::Buy  => aggressor bought (taker buy)
    // aggressor == OrderSide::Sell => aggressor sold (taker sell)
    void processAggressorTrade(OrderSide aggressor, const std::string& instrument, const Trade& trade);

    std::optional<Position> getPosition(const std::string& instrument) const;
    const std::unordered_map<std::string, Position>& getPositions() const;

private:
    std::unordered_map<std::string, Position> positions_;
};
