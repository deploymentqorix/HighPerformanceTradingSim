#include "audit_logger.h"
#include "command.h"
#include "order.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <ctime>

using json = nlohmann::json;

AuditLogger::AuditLogger(const std::string& filename) : log_file(filename, std::ios::app) {}

void AuditLogger::logCommand(const Command& c) {
    if (!log_file.is_open()) {
        return;
    }

    json j;
    j["timestamp"] = std::time(nullptr);
    j["command_type"] = "NEW_ORDER";
    j["instrument"] = c.instrument;

    // Create a sub-object for order details
    json order_details;
    order_details["id"] = c.order.orderId;
    order_details["price"] = c.order.price;
    order_details["quantity"] = c.order.quantity;
    order_details["side"] = (c.order.side == Side::BUY) ? "BUY" : "SELL";
    
    j["order"] = order_details;

    log_file << j.dump() << std::endl;
}