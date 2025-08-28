#include "order.h"

// 6-arg constructor (default display_quantity = qty)
Order::Order(int id, const std::string& instr, OrderType t, OrderSide s, double p, int q)
    : id(id),
      instrument(instr),
      type(t),
      side(s),
      price(p),
      quantity(q),
      remaining_quantity(q),
      display_quantity(q) {}

// 7-arg constructor
Order::Order(int id, const std::string& instr, OrderType t, OrderSide s, double p, int q, int disp)
    : id(id),
      instrument(instr),
      type(t),
      side(s),
      price(p),
      quantity(q),
      remaining_quantity(q),
      display_quantity(disp) {}
