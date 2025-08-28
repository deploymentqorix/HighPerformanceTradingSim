#ifndef ENGINE_H
#define ENGINE_H

#include "order_book.h"
#include "order.h"
#include "trade.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

// ============================
// Requirement Mapping
// QCSIDM_SRS_028 -> Event-driven matching engine
// QCSIDM_SRS_029 -> Price-time priority
// QCSIDM_SRS_030 -> Partial fills supported
// QCSIDM_SRS_031 -> Cancels
// QCSIDM_SRS_032 -> Replaces
// QCSIDM_SRS_033 -> Market orders
// QCSIDM_SRS_034 -> Limit orders
// QCSIDM_SRS_035 -> Matching trades & residual resting orders
// QCSIDM_SRS_036 -> Generate market data events
// QCSIDM_SRS_037 -> Risk pre-trade checks
// QCSIDM_SRS_038 -> Positions & PnL bookkeeping
// QCSIDM_SRS_039 -> Deterministic single-threaded loop
// ============================

class Engine {
public:
    Engine();

    // Submit new order (limit / market)
    std::vector<Trade> submitOrder(const Order& order);

    // Cancel existing order
    bool cancelOrder(int orderId);

    // Replace existing order (update qty/price)
    bool replaceOrder(int orderId, double newPrice, int newQty);

    // Risk check before accepting order
    bool riskCheck(const Order& order);

    // Accounting (PnL, positions)
    void updateAccounting(const Trade& trade);

    // Run engine loop (single-threaded)
    void run();

private:
    std::unordered_map<std::string, OrderBook> books;   // symbol -> order book
    std::unordered_map<int, Order> orderLookup;         // orderId -> order
    std::unordered_map<std::string, double> positions;  // symbol -> position
    double pnl;
};

#endif // ENGINE_H
