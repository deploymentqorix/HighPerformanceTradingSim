#pragma once
#include <string>

enum class OrderSide { Buy, Sell };
enum class OrderType { Market, Limit };

struct Order {
    int id;
    std::string instrument;
    OrderType type;
    OrderSide side;
    double price;
    int quantity;
    int remaining_quantity;
    int display_quantity;  // ✅ added for 7-arg constructor

    Order() = default;
    Order(int id, const std::string& instr, OrderType type, OrderSide side, double price, int qty);
    Order(int id, const std::string& instr, OrderType type, OrderSide side, double price, int qty, int display_qty);
};
