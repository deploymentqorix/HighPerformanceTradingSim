#include "risk_manager.h"
#include "order.h"
#include "trade.h"
#include <iostream>

RiskManager::RiskManager(int maxOrderSize, double maxExposure)
    : maxOrderSize(maxOrderSize), maxExposure(maxExposure) {}

bool RiskManager::validateOrder(const Order& order) {
    // CHANGED: from order.qty to order.quantity
    if (order.quantity > maxOrderSize) {
        std::cerr << "Risk Check Failed: Order size " << order.quantity
                  << " exceeds max size " << maxOrderSize << std::endl;
        return false;
    }
    // TODO: Add more checks (e.g., exposure)
    return true;
}

double RiskManager::calculateExposure(const std::vector<Trade>& trades) {
    double exposure = 0.0;
    for (const auto& t : trades) {
        // CHANGED: from t.qty to t.quantity
        exposure += t.quantity * t.price;
    }
    return exposure;
}

double RiskManager::calculatePnL(const std::vector<Trade>& trades, double marketPrice) {
    double pnl = 0.0;
    for (const auto& t : trades) {
        // This assumes all trades are BUYs for simplicity
        // CHANGED: from t.qty to t.quantity
        pnl += (marketPrice - t.price) * t.quantity;
    }
    return pnl;
}