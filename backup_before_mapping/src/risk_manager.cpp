#include "risk_manager.h"

RiskManager::RiskManager(int max_order_size) : max_order_size_(max_order_size) {}

bool RiskManager::checkOrder(const Order& order) const {
    return order.quantity > 0 && order.quantity <= max_order_size_;
}
