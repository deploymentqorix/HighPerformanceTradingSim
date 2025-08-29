#pragma once
#include <string>

// Assuming Side is an enum defined somewhere, like this:
enum class Side { BUY, SELL };

struct Order {
    // --- Member Variables ---
    std::string orderId; // CHANGED: from "id" to "orderId" to match your .cpp file
    Side side;
    double price;
    int quantity;
    long timestamp;

    // --- Constructors ---
    Order() = default;
    Order(std::string id, Side side, double price, int quantity, long timestamp);

    // --- Member Functions ---
    std::string toString() const;
};