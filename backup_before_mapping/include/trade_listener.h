#pragma once
#include "trade.h"
#include <string>

class TradeListener {
public:
    virtual ~TradeListener() = default;
    virtual void onTrade(const std::string& instrument, const Trade& trade) = 0;
};
