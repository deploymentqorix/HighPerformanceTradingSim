#pragma once
#include <string>
#include <vector>
#include <functional>
#include <map>

// Requirement Mapping
// QCSIDM_SRS_047 - Simulated market data
// QCSIDM_SRS_048 - Multiple symbols support
// QCSIDM_SRS_049 - Configurable frequency
// QCSIDM_SRS_050 - Publish to listeners

struct MarketTick {
    std::string symbol;
    double price;
    int volume;
};

class MarketDataGenerator {
public:
    MarketDataGenerator();

    void addSymbol(const std::string& symbol);  // SRS_048
    void setFrequency(int ms);                  // SRS_049
    void start();                               // SRS_047
    void stop();
    
    void subscribe(std::function<void(const MarketTick&)> listener); // SRS_050

private:
    std::vector<std::string> symbols;
    int frequencyMs;
    bool running;

    std::vector<std::function<void(const MarketTick&)>> listeners;

    void generateLoop(); // Internal thread
};
