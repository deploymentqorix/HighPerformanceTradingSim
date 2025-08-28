#pragma once
#include "matching_engine.h"
#include <atomic>
#include <thread>

class MarketDataGenerator {
public:
    explicit MarketDataGenerator(MatchingEngine& engine);
    void run();
    void stop();

private:
    MatchingEngine& engine_;
    std::atomic<bool> is_running_;
};
