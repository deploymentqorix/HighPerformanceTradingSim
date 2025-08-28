#include "market_data_generator.h"
#include <cstdlib>
#include <ctime>

MarketDataGenerator::MarketDataGenerator() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

MarketTick MarketDataGenerator::generate(const std::string& instrument) {
    MarketTick tick;
    tick.instrument = instrument;
    tick.price = 100.0 + (std::rand() % 1000) / 10.0;  // random price 100–200
    tick.quantity = 1 + (std::rand() % 100);           // random qty 1–100
    tick.timestamp = std::time(nullptr);
    return tick;
}
