#pragma once
#include <vector>

struct PriceLevel {
    double price;
    int quantity;
};

struct MarketDepth {
    std::vector<PriceLevel> bids;
    std::vector<PriceLevel> asks;
};
