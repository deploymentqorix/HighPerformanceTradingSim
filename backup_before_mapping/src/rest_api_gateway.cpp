#include "rest_api_gateway.h"
#include "matching_engine.h"
#include "logger.h"
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

RestApiGateway::RestApiGateway(const std::string& host, int port, MatchingEngine& engine)
    : host_(host), port_(port), engine_(engine), server_(std::make_unique<httplib::Server>()) {}

RestApiGateway::~RestApiGateway() {
    stop();
    if (server_thread_.joinable()) {
        server_thread_.join();
    }
}

void RestApiGateway::stop() {
    if (server_ && server_->is_running()) {
        server_->stop();
    }
}

void RestApiGateway::run() {
    if (!server_) return;

    server_->set_mount_point("/", "./web");

    server_->Get("/status", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("{\"status\": \"running\"}", "application/json");
    });

    server_->Get("/positions", [&](const httplib::Request&, httplib::Response& res) {
        std::string report = engine_.getPositionsReport();
        res.set_content(report, "text/plain");
    });

    server_->Get(R"(/depth/(\w+))", [&](const httplib::Request& req, httplib::Response& res) {
        auto instrument = req.matches[1].str();
        std::string report = engine_.getMarketDepthReport(instrument);
        res.set_content(report, "text/plain");
    });
    
    server_->Get("/metrics", [&](const httplib::Request&, httplib::Response& res) {
        std::string report = engine_.getMetricsReport();
        res.set_content(report, "application/json");
    });

    // --- NEW: Endpoint to list all instruments ---
    server_->Get("/instruments", [&](const httplib::Request&, httplib::Response& res) {
        auto instruments = engine_.getTradedInstruments();
        json j(instruments);
        res.set_content(j.dump(4), "application/json");
    });

    server_->Post("/order", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);
            int id = 0;
            std::string instrument = data["instrument"];
            OrderSide side = (data["side"] == "BUY") ? OrderSide::Buy : OrderSide::Sell;
            double price = data["price"];
            int qty = data["quantity"];
            int display_qty = data.value("display_quantity", qty); // Allow optional display_qty

            engine_.postCommand({AddOrderCommand{{id, instrument, OrderType::Limit, side, price, qty, display_qty}}});
            res.set_content("{\"status\": \"OK\"}", "application/json");
        } catch (...) {
            res.set_content("{\"status\": \"ERROR\", \"message\": \"Invalid order format\"}", "application/json");
            res.status = 400;
        }
    });
    
    // --- NEW: Endpoint to cancel an order ---
    server_->Post("/cancel", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);
            int orderId = data["orderId"];
            std::string instrument = data["instrument"];

            engine_.postCommand({CancelOrderCommand{orderId, instrument}});
            res.set_content("{\"status\": \"OK\"}", "application/json");
        } catch (...) {
            res.set_content("{\"status\": \"ERROR\", \"message\": \"Invalid cancel format\"}", "application/json");
            res.status = 400;
        }
    });


    Logger::log(LogLevel::INFO, "REST API Gateway starting on " + host_ + ":" + std::to_string(port_));
    
    server_thread_ = std::thread([this]() {
        if (!server_->listen(host_.c_str(), port_)) {
            Logger::log(LogLevel::ERROR, "Failed to start REST API server.");
        }
        Logger::log(LogLevel::INFO, "REST API Gateway has stopped.");
    });
}