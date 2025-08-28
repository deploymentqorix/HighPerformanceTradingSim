#include "audit_logger.h"
#include <stdexcept>
#include <nlohmann/json.hpp>  // ✅ For JSON serialization

using json = nlohmann::json;

AuditLogger::AuditLogger(const std::string& filepath)
    : audit_file_(filepath, std::ios::app) {
    if (!audit_file_.is_open()) {
        throw std::runtime_error("Failed to open audit log file: " + filepath);
    }
}

AuditLogger::~AuditLogger() {
    if (audit_file_.is_open()) {
        audit_file_.close();
    }
}

void AuditLogger::logCommand(const Command& command) {
    json j;

    if (std::holds_alternative<AddOrderCommand>(command.data)) {
        const auto& cmd = std::get<AddOrderCommand>(command.data);
        j["type"] = "AddOrder";
        j["order"] = {
            {"id", cmd.order.id},
            {"instrument", cmd.order.instrument},
            {"side", cmd.order.side == OrderSide::Buy ? "Buy" : "Sell"},
            {"price", cmd.order.price},
            {"quantity", cmd.order.quantity}
        };
    }
    else if (std::holds_alternative<CancelOrderCommand>(command.data)) {
        const auto& cmd = std::get<CancelOrderCommand>(command.data);
        j["type"] = "CancelOrder";
        j["orderId"] = cmd.orderId;
        j["instrument"] = cmd.instrument;
    }
    else if (std::holds_alternative<ShutdownCommand>(command.data)) {
        j["type"] = "Shutdown";
    }

    audit_file_ << j.dump() << std::endl;
}
