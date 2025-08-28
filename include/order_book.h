#pragma once
#include <map>
#include <queue>
#include <unordered_map>
#include "order.h"

// REQ-010: OrderBook class to manage bids/asks
class OrderBook {
public:
    // REQ-011: Add new order
    bool addOrder(const Order& order);

    // REQ-012: Cancel order by ID
    bool cancelOrder(int orderId);

    // REQ-013: Replace order with new params
    bool replaceOrder(int orderId, double newPrice, int newQty);

    // REQ-014: Match market order against book
    void matchMarket(Order& order);

private:
    // REQ-015: Bids stored in price → FIFO queue
    std::map<double, std::queue<Order>> bids;

    // REQ-016: Asks stored in price → FIFO queue
    std::map<double, std::queue<Order>> asks;

    // REQ-017: Order lookup for O(1) cancel/replace
    std::unordered_map<int, Order*> orderLookup;
};
