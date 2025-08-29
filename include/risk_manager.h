#pragma once
#include <vector>

// Forward-declare the structs
struct Order;
struct Trade;

class RiskManager {
public:
    // CORRECTED: The first parameter is now 'int' to match the .cpp file.
    RiskManager(int maxOrderSize, double maxExposure);

    bool validateOrder(const Order& order);
    double calculateExposure(const std::vector<Trade>& trades);
    double calculatePnL(const std::vector<Trade>& trades, double marketPrice);

private:
    int maxOrderSize;
    double maxExposure;
};