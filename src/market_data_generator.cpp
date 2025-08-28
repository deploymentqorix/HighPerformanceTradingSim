#include "market_data_generator.h"
#include <thread>
#include <chrono>
#include <random>
#include <iostream>

MarketDataGenerator::MarketDataGenerator()
    : frequencyMs(1000), running(false) {}

void MarketDataGenerator::addSymbol(const std::string& symbol) {
    symbols.push_back(symbol); // SRS_048
}

void MarketDataGenerator::setFrequency(int ms) {
    frequencyMs = ms; // SRS_049
}

void MarketDataGenerator::start() {
    running = true;
    std::thread([this]() { generateLoop(); }).detach(); // SRS_047
}

void MarketDataGenerator::stop() {
    running = false;
}

void MarketDataGenerator::subscribe(std::function<void(const MarketTick&)> listener) {
    listeners.push_back(listener); // SRS_050
}

void MarketDataGenerator::generateLoop() {
    std::default_random_engine gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(100.0, 200.0);

    while (running) {
        for (const auto& sym : symbols) {
            MarketTick tick{sym, dist(gen), rand() % 1000};

            // Notify all listeners
            for (auto& l : listeners) {
                l(tick);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(frequencyMs));
    }
}
