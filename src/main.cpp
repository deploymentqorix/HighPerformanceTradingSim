#include "matching_engine.h"
#include "zeromq_gateway.h"
#include "rest_api_gateway.h"
#include "config.h" // RE-ADDED: This was the cause of the new error. My apologies.
#include "trade_listener.h"
#include "market_data_listener.h"
#include <iostream>
#include <thread>
#include <signal.h>
#include <memory>
#include <sstream>

std::unique_ptr<RestApiGateway> rest_gateway_ptr;

class ConsoleTradeListener : public TradeListener {
public:
    void onTrade(const std::string& instrument, const Trade& trade) override {
        std::stringstream ss;
        ss << "[" << instrument << " Trade] BuyID=" << trade.buy_order_id
           << ", SellID=" << trade.sell_order_id
           << ", Qty=" << trade.quantity
           << ", Price=" << trade.price;
        std::cout << ss.str() << std::endl;
    }
};

class ZmqMarketDataListener : public MarketDataListener {
public:
    explicit ZmqMarketDataListener(ZeroMQGateway& gateway) : gateway_(gateway) {}
    void onMarketDepthUpdate(const std::string& instrument, const MarketDepth& depth) override {
    }
private:
    ZeroMQGateway& gateway_;
};

void signal_handler(int signal) {
    if (rest_gateway_ptr) {
        rest_gateway_ptr->stop();
    }
    exit(signal);
}

int main() {
    Config config("config.json");
    signal(SIGINT, signal_handler);

    MatchingEngine engine(config.max_order_size);
    ZeroMQGateway zmq_gateway("tcp://*:5555", "tcp://*:5556");
    rest_gateway_ptr = std::make_unique<RestApiGateway>("localhost", 8080, engine);

    ConsoleTradeListener trade_listener;
    ZmqMarketDataListener md_listener(zmq_gateway);

    engine.addListener(&trade_listener);
    engine.addMarketDataListener(&md_listener);

    std::thread rest_thread(&RestApiGateway::run, rest_gateway_ptr.get());
    std::thread zmq_thread(&ZeroMQGateway::start, &zmq_gateway);
    std::thread engine_thread(&MatchingEngine::run, &engine);

    std::cout << "System is running. Press Ctrl+C to exit." << std::endl;

    engine_thread.join();
    zmq_thread.join();
    rest_thread.join();

    return 0;
}