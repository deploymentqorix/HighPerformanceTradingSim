#pragma once
#include "order.h"

class RiskManager {
public:
    explicit RiskManager(int max_order_size);
    bool checkOrder(const Order& order) const;

private:
    int max_order_size_;
};
