#include "engine.h"
#include <iostream>

Engine::Engine() : pnl(0.0) {}

std::vector<Trade> Engine::submitOrder(const Order& order) {
    std::vector<Trade> trades;

    // Risk check
    if (!riskCheck(order)) {
        std::cout << "Risk check failed for order " << order.id << "\n";
        return trades;
    }

    // Ensure book exists
    if (books.find(order.symbol) == books.end()) {
        books[order.symbol] = OrderBook(order.symbol);
    }

    // Match order
    trades = books[order.symbol].match(order);

    // Save order if it’s still active
    if (order.status == OrderStatus::OPEN) {
        orderLookup[order.id] = order;
    }

    // Update accounting
    for (auto& t : trades) {
        updateAccounting(t);
    }

    return trades;
}

bool Engine::cancelOrder(int orderId) {
    auto it = orderLookup.find(orderId);
    if (it == orderLookup.end()) return false;

    std::string sym = it->second.symbol;
    return books[sym].cancel(orderId);
}

bool Engine::replaceOrder(int orderId, double newPrice, int newQty) {
    auto it = orderLookup.find(orderId);
    if (it == orderLookup.end()) return false;

    std::string sym = it->second.symbol;
    return books[sym].replace(orderId, newPrice, newQty);
}

bool Engine::riskCheck(const Order& order) {
    // Example: block orders > 1,000,000 qty
    if (order.quantity > 1000000) return false;
    return true;
}

void Engine::updateAccounting(const Trade& trade) {
    double value = trade.price * trade.quantity;
    if (trade.buyOrderId != -1) {
        positions[trade.symbol] += trade.quantity;
        pnl -= value;
    }
    if (trade.sellOrderId != -1) {
        positions[trade.symbol] -= trade.quantity;
        pnl += value;
    }
}

void Engine::run() {
    std::cout << "Engine running (single-threaded)...\n";
    // In real use: pull orders from a queue and process in batch
}
