// src/main_backend_test.cpp

#include "zeromq_gateway.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

// These should match the endpoints in your MainWindow
const std::string MARKET_DATA_ENDPOINT = "tcp://*:5555";
const std::string ORDER_ENDPOINT = "tcp://*:5556";

// Dummy order handler
void handleOrder(const std::string& order) {
    std::cout << "[Backend] Received Order: " << order << std::endl;
}

int main() {
    // This gateway will PUBLISH Market Data
    zmq::context_t pub_context(1);
    zmq::socket_t marketDataPublisher(pub_context, ZMQ_PUB);
    marketDataPublisher.bind(MARKET_DATA_ENDPOINT);

    // This gateway will SUBSCRIBE to Orders
    zmq::context_t sub_context(1);
    zmq::socket_t orderSubscriber(sub_context, ZMQ_SUB);
    orderSubscriber.bind(ORDER_ENDPOINT);
    orderSubscriber.setsockopt(ZMQ_SUBSCRIBE, "LIMIT", 5);

    std::cout << "[Backend] Publishing market data on " << MARKET_DATA_ENDPOINT << std::endl;
    std::cout << "[Backend] Listening for orders on " << ORDER_ENDPOINT << std::endl;

    // Thread to listen for incoming orders from the UI
    std::thread orderThread([&]() {
        while (true) {
            zmq::message_t msg;
            if(orderSubscriber.recv(msg)) {
                handleOrder(std::string(static_cast<char*>(msg.data()), msg.size()));
            }
        }
    });
    orderThread.detach(); // Let the thread run in the background

    // Main thread to publish dummy market data every second
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> price_dist(100.0, 102.0);
    std::uniform_int_distribution<> qty_dist(1, 10);

    while (true) {
        double price = price_dist(gen);
        int qty = qty_dist(gen);

        // Format: "TRADE,PRICE,QTY"
        std::string trade_data = "TRADE," + std::to_string(price) + "," + std::to_string(qty);
        
        marketDataPublisher.send(zmq::buffer(trade_data));
        std::cout << "[Backend] Published Trade: " << trade_data << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}