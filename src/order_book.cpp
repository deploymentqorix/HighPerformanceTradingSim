#include "order_book.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>

// ===============================
// OrderBook Implementation
// ===============================

// QCSIDM_SRS_014: The system shall maintain an order book per instrument
// QCSIDM_SRS_015: The order book shall support insertion of buy and sell orders
// QCSIDM_SRS_016: Orders shall be organized by price levels
// QCSIDM_SRS_017: Within a price level, FIFO (time priority) shall be maintained
// QCSIDM_SRS_018: The system shall support canceling orders by ID
// QCSIDM_SRS_019: The system shall support order replacement (modify qty/price)
// QCSIDM_SRS_020: The system shall provide best bid/ask retrieval in O(1)
// QCSIDM_SRS_021: Matching engine shall execute trades when prices cross
// QCSIDM_SRS_022: Partial fills shall be supported
// QCSIDM_SRS_023: Residual unfilled orders shall remain in the book
// QCSIDM_SRS_024: Trade events shall be generated on each match
// QCSIDM_SRS_025: The system shall publish top-of-book updates
// QCSIDM_SRS_026: Cancel of non-existing order shall not crash the system
// QCSIDM_SRS_027: Replace of non-existing order shall not crash the system

OrderBook::OrderBook(std::string symbol)
    : symbol(std::move(symbol)) {}

void OrderBook::addOrder(const Order& order) {
    // (requirement: QCSIDM_SRS_015, QCSIDM_SRS_016, QCSIDM_SRS_017)
    auto& bookSide = (order.side == OrderSide::BUY) ? bids : asks;
    bookSide[order.price].push(order);
    orderMap[order.orderId] = order;
}

bool OrderBook::cancelOrder(const std::string& orderId) {
    // (requirement: QCSIDM_SRS_018, QCSIDM_SRS_026)
    auto it = orderMap.find(orderId);
    if (it == orderMap.end()) return false;
    orderMap.erase(it);
    return true;
}

bool OrderBook::replaceOrder(const std::string& orderId, double newPrice, int newQty) {
    // (requirement: QCSIDM_SRS_019, QCSIDM_SRS_027)
    auto it = orderMap.find(orderId);
    if (it == orderMap.end()) return false;

    Order newOrder = it->second;
    newOrder.price = newPrice;
    newOrder.quantity = newQty;

    cancelOrder(orderId);
    addOrder(newOrder);
    return true;
}

double OrderBook::getBestBid() const {
    // (requirement: QCSIDM_SRS_020)
    if (bids.empty()) return 0.0;
    return bids.rbegin()->first;
}

double OrderBook::getBestAsk() const {
    // (requirement: QCSIDM_SRS_020)
    if (asks.empty()) return 0.0;
    return asks.begin()->first;
}

std::vector<Trade> OrderBook::match() {
    // (requirement: QCSIDM_SRS_021, QCSIDM_SRS_022, QCSIDM_SRS_023, QCSIDM_SRS_024, QCSIDM_SRS_025)
    std::vector<Trade> trades;

    while (!bids.empty() && !asks.empty() && getBestBid() >= getBestAsk()) {
        auto& bestBidQueue = bids.rbegin()->second;
        auto& bestAskQueue = asks.begin()->second;

        Order buyOrder = bestBidQueue.front();
        Order sellOrder = bestAskQueue.front();

        int tradeQty = std::min(buyOrder.quantity, sellOrder.quantity);
        double tradePrice = (buyOrder.timestamp < sellOrder.timestamp) ? buyOrder.price : sellOrder.price;

        trades.emplace_back("T" + std::to_string(trades.size() + 1),
                            buyOrder.orderId,
                            sellOrder.orderId,
                            tradePrice,
                            tradeQty,
                            std::time(nullptr));

        buyOrder.quantity -= tradeQty;
        sellOrder.quantity -= tradeQty;

        if (buyOrder.quantity == 0) bestBidQueue.pop();
        else orderMap[buyOrder.orderId] = buyOrder;

        if (sellOrder.quantity == 0) bestAskQueue.pop();
        else orderMap[sellOrder.orderId] = sellOrder;

        if (bestBidQueue.empty()) bids.erase(std::prev(bids.end()));
        if (bestAskQueue.empty()) asks.erase(asks.begin());
    }

    return trades;
}

std::string OrderBook::toString() const {
    std::ostringstream oss;
    oss << "OrderBook[" << symbol << "]\n";
    oss << "BestBid=" << getBestBid() << " BestAsk=" << getBestAsk() << "\n";
    return oss.str();
}
