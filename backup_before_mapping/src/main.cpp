#include "matching_engine.h"
#include "zeromq_gateway.h"
#include "rest_api_gateway.h"
#include "logger.h"
#include "trade_listener.h"
#include "market_data_listener.h"
#include "command.h"
#include "config.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <sstream>
#include <csignal>

class ConsoleTradeListener : public TradeListener {
public:
    void onTrade(const std::string& instrument, const Trade& trade) override {
        std::stringstream ss;
        ss << "[" << instrument << " Trade] ID=" << trade.trade_id << ", BuyID=" << trade.buy_order_id
           << ", SellID=" << trade.sell_order_id << ", Qty=" << trade.quantity << ", Price=" << trade.price;
        Logger::log(LogLevel::INFO, ss.str());
    }
};

class ConsoleMarketDataListener : public MarketDataListener {
public:
    void onMarketDepthUpdate(const std::string& instrument, const MarketDepth& depth) override {
        std::stringstream ss;
        ss << "\n--- " << instrument << " MARKET DEPTH UPDATE ---\n";
        for (auto it = depth.asks.rbegin(); it != depth.asks.rend(); ++it) {
            ss << std::fixed << std::setprecision(2) << "       ASK: " << it->price << "  Qty: " << it->quantity << "\n";
        }
        ss << "---------------------------\n";
        for (const auto& level : depth.bids) {
            ss << std::fixed << std::setprecision(2) << "BID: " << level.price << "  Qty: " << level.quantity << "\n";
        }
        ss << "---------------------------";
        Logger::log(LogLevel::INFO, ss.str());
    }
};

RestApiGateway* rest_gateway_ptr = nullptr;
void signal_handler(int signal) {
    Logger::log(LogLevel::INFO, "Shutdown signal received.");
    if(rest_gateway_ptr) {
        rest_gateway_ptr->stop();
    }
}

int main() {
    Config config("config.json");
    MatchingEngine engine(config.max_order_size);
    ZeroMQGateway zmq_gateway("tcp://*:5555", engine);
    RestApiGateway rest_gateway("localhost", 8080, engine);
    rest_gateway_ptr = &rest_gateway;

    signal(SIGINT, signal_handler);
    
    ConsoleTradeListener trade_listener;
    ConsoleMarketDataListener md_listener;

    engine.addListener(&trade_listener);
    engine.addMarketDataListener(&md_listener);

    rest_gateway.run();
    
    std::thread engine_thread(&MatchingEngine::run, &engine);
    std::thread zmq_gateway_thread(&ZeroMQGateway::run, &zmq_gateway);

    Logger::log(LogLevel::INFO, "Engine and Gateways are running. Press Ctrl+C to exit.");
    
    engine_thread.join();
    zmq_gateway_thread.join();

    return 0;
}