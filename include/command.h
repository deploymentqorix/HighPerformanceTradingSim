#pragma once
#include <string>
#include "order.h"

struct Command {
    std::string instrument;
    Order order;  // already defined in order.h
};
