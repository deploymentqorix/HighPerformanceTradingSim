#pragma once
#include "order_book.h"
#include <vector>
#include <functional>

// REQ-040: Trade struct for fills
struct Trade {
    int buyOrderId;   // REQ-041: Buyer ID
    int sellOrderId;  // REQ-042: Seller ID
    double price;     // REQ-043: Execution price
    int qty;          // REQ-044: Execution qty
};

// REQ-045: MatchingEngine class
class MatchingEngine {
public:
    MatchingEngine();

    // REQ-046: Submit new order
    void submitOrder(const Order& order);

    // REQ-047: Cancel order
    bool cancelOrder(int orderId);

    // REQ-048: Replace order
    bool replaceOrder(int orderId, double newPrice, int newQty);

    // REQ-049: Subscribe to trade events
    void onTrade(std::function<void(const Trade&)> callback);

private:
    OrderBook orderBook;   // REQ-050: Internal order book
    std::vector<std::function<void(const Trade&)>> tradeListeners; // REQ-051: Subscribers

    // REQ-052: Internal helper to publish trades
    void publishTrade(const Trade& t);
};
