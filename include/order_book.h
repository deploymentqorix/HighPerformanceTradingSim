// include/order_book.h

#pragma once
#include <string>
#include <vector>
#include <map>

// CHANGED: We now include the full definitions instead of forward-declaring.
// This gives the compiler the "blueprint" it needs for std::vector and std::map.
#include "order.h"
#include "trade.h"

// QCSIDM_SRS_020 - Order Book Management
class OrderBook {
public:
    // QCSIDM_SRS_021 - Constructor for a specific symbol
    OrderBook(const std::string& symbol); // Pass by const referencel);
    OrderBook() = default;

    // QCSIDM_SRS_022 - Add new order to the book
    void addOrder(const Order& order);

    // QCSIDM_SRS_023 - Cancel existing order by ID
    bool cancelOrder(int orderId);

    // QCSIDM_SRS_024 - Replace/modify existing order by ID
    bool replaceOrder(int orderId, double newPrice, int newQty);
    
    // QCSIDM_SRS_025 - Retrieve best bid price
    double getBestBid() const;

    // QCSIDM_SRS_026 - Retrieve best ask price
    double getBestAsk() const;

    // QCSIDM_SRS_027 - Core matching logic to generate trades
    std::vector<Trade> match();

    // QCSIDM_SRS_028 - Get string representation of the book state
    std::string toString() const;

private:
    std::string symbol;
    // QCSIDM_SRS_029 - Data structures for bids and asks
    std::map<double, std::vector<Order>> bids;
    std::map<double, std::vector<Order>> asks;
};