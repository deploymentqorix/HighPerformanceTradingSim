#pragma once

// REQ-001: Define order side (Buy/Sell)
// REQ-002: Define order type (Limit/Market)
enum class Side { Buy, Sell };
enum class OrderType { Limit, Market };

// REQ-003: Basic Order struct with id, side, type, price, qty
struct Order {
    int id;             // REQ-004: Unique Order ID
    Side side;          // REQ-005: Order direction (Buy/Sell)
    OrderType type;     // REQ-006: Order type (Limit/Market)
    double price;       // REQ-007: Price field
    int qty;            // REQ-008: Quantity field
};
