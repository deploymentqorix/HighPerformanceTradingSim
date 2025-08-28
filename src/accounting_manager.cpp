#include "accounting_manager.h"
#include "logger.h"
#include <algorithm>

/*
  Implementation notes:
  - We maintain a per-instrument Position (net qty, average_price, realized_pnl).
  - When aggressor is BUY:
      * If we had net short (qty < 0) we close short first -> realized pnl increases.
      * Remaining buy qty increases long exposure and updates avg price.
  - When aggressor is SELL:
      * If we had net long (qty > 0) we close long first -> realized pnl increases.
      * Remaining sell qty increases short exposure and sets short avg price.
  - This is a simplified aggregated accounting (no per-account ledger). It's sufficient
    to show positions and realized P&L at instrument granularity (QCSIDM_SRS_013/014).
*/

void AccountingManager::onTrade(const std::string& instrument, const Trade& trade) {
    // Keep compatibility with generic listeners; we do not compute accounting here.
    // MatchingEngine will call processAggressorTrade(...) for accounting purposes.
    (void)instrument;
    (void)trade;
}

void AccountingManager::processAggressorTrade(OrderSide aggressor, const std::string& instrument, const Trade& trade) {
    auto& pos = positions_[instrument];
    int q = trade.quantity;
    double px = trade.price;

    if (q <= 0) return;

    if (aggressor == OrderSide::Buy) {
        // BUY as aggressor: buy to close short or buy to open/increase long
        if (pos.quantity < 0) {
            // we have a short position - close it first
            int close_qty = std::min(q, -pos.quantity);
            // realized pnl for short = (short avg price - execution price) * closed_qty
            pos.realized_pnl += (pos.average_price - px) * static_cast<double>(close_qty);
            pos.quantity += close_qty; // move toward zero
            q -= close_qty;
            if (pos.quantity == 0) pos.average_price = 0.0;
        }
        // remaining buys increase long position
        if (q > 0) {
            double total_cost = pos.average_price * static_cast<double>(pos.quantity) + px * static_cast<double>(q);
            pos.quantity += q;
            pos.average_price = total_cost / static_cast<double>(pos.quantity);
        }
    } else { // aggressor == Sell
        // SELL as aggressor: sell to close long or sell to open/increase short
        if (pos.quantity > 0) {
            int close_qty = std::min(q, pos.quantity);
            // realized pnl for long = (execution price - avg price) * closed_qty
            pos.realized_pnl += (px - pos.average_price) * static_cast<double>(close_qty);
            pos.quantity -= close_qty;
            q -= close_qty;
            if (pos.quantity == 0) pos.average_price = 0.0;
        }
        // remaining sells increase short position
        if (q > 0) {
            // If we already have a short, update its average price (weighted average of prices)
            if (pos.quantity < 0) {
                int existing_short = -pos.quantity;
                double total_proceeds = pos.average_price * static_cast<double>(existing_short) + px * static_cast<double>(q);
                int new_short_qty = existing_short + q;
                pos.average_price = total_proceeds / static_cast<double>(new_short_qty);
            } else {
                // opening new short position: set avg to this price
                pos.average_price = px;
            }
            pos.quantity -= q; // more negative => larger short
        }
    }

    Logger::log(LogLevel::INFO, std::string("Accounting: ") + instrument
                + " qty=" + std::to_string(pos.quantity)
                + " avg=" + std::to_string(pos.average_price)
                + " rPnL=" + std::to_string(pos.realized_pnl));
}

std::optional<Position> AccountingManager::getPosition(const std::string& instrument) const {
    auto it = positions_.find(instrument);
    if (it != positions_.end()) return it->second;
    return std::nullopt;
}

const std::unordered_map<std::string, Position>& AccountingManager::getPositions() const {
    return positions_;
}
