#pragma once
#include "order.h"
#include <variant>
#include <string>

struct AddOrderCommand {
    Order order;
};

struct CancelOrderCommand {
    int orderId;
    std::string instrument;
};

struct ShutdownCommand {};

using CommandData = std::variant<AddOrderCommand, CancelOrderCommand, ShutdownCommand>;

struct Command {
    CommandData data;
};
