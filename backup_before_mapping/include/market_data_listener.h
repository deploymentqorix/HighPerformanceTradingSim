#pragma once
#include "market_data.h"
#include <string>

class MarketDataListener {
public:
    virtual ~MarketDataListener() = default;
    virtual void onMarketDepthUpdate(const std::string& instrument, const MarketDepth& depth) = 0;
};
