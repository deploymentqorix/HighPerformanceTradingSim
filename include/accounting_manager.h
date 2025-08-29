#pragma once
#include <string>
#include <map>
#include "order.h" // ADDED: To get the definition for 'Side' and 'Trade'
#include "trade.h"

class AccountingManager {
public:
    // ... other functions ...

    // CORRECTED: Changed 'OrderSide' to the correct type 'Side'
    void processAggressorTrade(Side aggressor, const std::string& instrument, const Trade& trade);

private:
    // ... member variables ...
};