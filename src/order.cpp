#include "order.h"
#include <sstream>

// ===============================
// Order Implementation
// ===============================

// QCSIDM_SRS_001: System shall support order creation
// QCSIDM_SRS_002: Orders shall be validated before processing
// QCSIDM_SRS_003: Orders shall include attributes (id, side, price, qty, timestamp)
// QCSIDM_SRS_009: System shall log all order events (via toString helper)

Order::Order(std::string id, Side side, double price, int quantity, long timestamp)
    : orderId(std::move(id)), side(side), price(price), quantity(quantity), timestamp(timestamp) {
    // (requirement: QCSIDM_SRS_002)
    if (price <= 0 || quantity <= 0) {
        throw std::invalid_argument("Invalid order parameters");
    }
}

std::string Order::toString() const {
    // (requirement: QCSIDM_SRS_009)
    std::ostringstream oss;
    oss << "Order[ID=" << orderId
        << ", Side=" << (side == Side::BUY ? "BUY" : "SELL")
        << ", Price=" << price
        << ", Qty=" << quantity
        << ", TS=" << timestamp
        << "]";
    return oss.str();
}
