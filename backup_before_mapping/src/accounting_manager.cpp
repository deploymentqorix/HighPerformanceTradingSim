#include "accounting_manager.h"
#include "logger.h"

void AccountingManager::onTrade(const std::string& instrument, const Trade& trade) {
    auto& pos = positions_[instrument];

    if (trade.quantity == 0) return;

    // Update average price (simplified FIFO-style accounting)
    double total_cost = pos.average_price * pos.quantity;
    total_cost += trade.price * trade.quantity;

    pos.quantity += trade.quantity;
    if (pos.quantity != 0) {
        pos.average_price = total_cost / pos.quantity;
    } else {
        pos.average_price = 0.0; // reset when flat
    }

    Logger::log(LogLevel::INFO,
        "Accounting updated for " + instrument +
        " | Qty: " + std::to_string(pos.quantity) +
        " | AvgPx: " + std::to_string(pos.average_price));
}

std::optional<Position> AccountingManager::getPosition(const std::string& instrument) const {
    auto it = positions_.find(instrument);
    if (it != positions_.end()) {
        return it->second;
    }
    return std::nullopt;
}

const std::unordered_map<std::string, Position>& AccountingManager::getPositions() const {
    return positions_;
}
