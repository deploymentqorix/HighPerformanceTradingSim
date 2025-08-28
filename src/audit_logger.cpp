#include "audit_logger.h"
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

/*
  Audit mapping:
  - QCSIDM_SRS_023: Logging of orders/trades to audit file
  - QCSIDM_SRS_024: Audit trail for replay (AuditReplayer reads these)
*/

AuditLogger::AuditLogger(const std::string& filepath) : audit_file_(filepath, std::ios::app) {
    if (!audit_file_.is_open()) throw std::runtime_error("Failed to open audit file");
}

AuditLogger::~AuditLogger() {
    if (audit_file_.is_open()) audit_file_.close();
}

void AuditLogger::logCommand(const Command& command) {
    if (!audit_file_.is_open()) return;
    json j;
    if (std::holds_alternative<AddOrderCommand>(command.data)) {
        const auto& c = std::get<AddOrderCommand>(command.data);
        j["type"] = "ADD";
        j["order"] = {
            {"id", c.order.id},
            {"instrument", c.order.instrument},
            {"side", (c.order.side==OrderSide::Buy) ? "BUY" : "SELL"},
            {"price", c.order.price},
            {"quantity", c.order.quantity}
        };
    } else if (std::holds_alternative<CancelOrderCommand>(command.data)) {
        const auto& c = std::get<CancelOrderCommand>(command.data);
        j["type"] = "CANCEL";
        j["orderId"] = c.orderId;
        j["instrument"] = c.instrument;
    } else if (std::holds_alternative<ShutdownCommand>(command.data)) {
        j["type"] = "SHUTDOWN";
    }
    audit_file_ << j.dump() << "\n";
    audit_file_.flush();
}
