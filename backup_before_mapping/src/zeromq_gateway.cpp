#include "zeromq_gateway.h"
#include "matching_engine.h"
#include "logger.h"
#include "command.h"
#include <zmq.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>

// Helper to split a string by whitespace
std::vector<std::string> split_str_zmq(const std::string& s) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (tokenStream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

ZeroMQGateway::ZeroMQGateway(const std::string& address, MatchingEngine& engine)
    : address_(address), engine_(engine), is_running_(false) {}

void ZeroMQGateway::stop() {
    is_running_ = false;
}

void ZeroMQGateway::run() {
    is_running_ = true;
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind(address_);
    Logger::log(LogLevel::INFO, "ZeroMQ Gateway listening on " + address_);
    int next_order_id = 1000;

    while (is_running_) {
        zmq::message_t request;
        auto result = socket.recv(request, zmq::recv_flags::dontwait);

        if (result && result.value() > 0) {
            std::string msg_str = std::string(static_cast<char*>(request.data()), request.size());
            auto tokens = split_str_zmq(msg_str);
            std::string reply_msg = "OK";

            if (tokens.empty()) {
                reply_msg = "ERROR: Empty command.";
            } else if (tokens[0] == "ADD" && (tokens.size() == 5 || tokens.size() == 6)) {
                try {
                    std::string instrument = tokens[1];
                    OrderSide side = (tokens[2] == "BUY") ? OrderSide::Buy : OrderSide::Sell;
                    double price = std::stod(tokens[3]);
                    int qty = std::stoi(tokens[4]);
                    int display_qty = (tokens.size() == 6) ? std::stoi(tokens[5]) : qty;
                    engine_.postCommand({AddOrderCommand{{next_order_id++, instrument, OrderType::Limit, side, price, qty, display_qty}}});
                } catch(...) {
                    reply_msg = "ERROR: Invalid ADD command format.";
                }
            } else if (tokens[0] == "CANCEL" && tokens.size() == 3) {
                try {
                    int orderId = std::stoi(tokens[1]);
                    std::string instrument = tokens[2];
                    engine_.postCommand({CancelOrderCommand{orderId, instrument}});
                } catch(...) {
                    reply_msg = "ERROR: Invalid CANCEL command format.";
                }
            } else if (tokens[0] == "INSTRUMENTS" && tokens.size() == 1) {
                auto instruments = engine_.getTradedInstruments();
                std::stringstream ss;
                ss << "Traded Instruments:\n";
                for(const auto& inst : instruments) {
                    ss << "- " << inst << "\n";
                }
                reply_msg = ss.str();
            } else if (tokens[0] == "POSITIONS" && tokens.size() == 1) {
                reply_msg = engine_.getPositionsReport();
            } else if (tokens[0] == "DEPTH" && tokens.size() == 2) {
                reply_msg = engine_.getMarketDepthReport(tokens[1]);
            } else {
                reply_msg = "ERROR: Unknown command or invalid arguments.";
            }
            socket.send(zmq::buffer(reply_msg), zmq::send_flags::none);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Logger::log(LogLevel::INFO, "ZeroMQ Gateway has stopped.");
}