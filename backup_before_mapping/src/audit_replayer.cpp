#include "audit_replayer.h"
#include "command.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

AuditReplayer::AuditReplayer(const std::string& filepath, MatchingEngine& engine)
    : filepath_(filepath), engine_(engine) {}

void AuditReplayer::run() {
    std::ifstream file(filepath_);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open audit log file: " + filepath_);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        json j = json::parse(line);
        Command cmd;

        std::string type = j["type"];
        if (type == "AddOrder") {
            Order order;
            order.id = j["order"]["id"];
            order.instrument = j["order"]["instrument"];
            order.side = (j["order"]["side"] == "Buy") ? OrderSide::Buy : OrderSide::Sell;
            order.price = j["order"]["price"];
            order.quantity = j["order"]["quantity"];
            cmd.data = AddOrderCommand{order};
        }
        else if (type == "CancelOrder") {
            CancelOrderCommand cancelCmd;
            cancelCmd.orderId = j["orderId"];
            cancelCmd.instrument = j["instrument"];
            cmd.data = cancelCmd;
        }
        else if (type == "Shutdown") {
            cmd.data = ShutdownCommand{};
        }

        engine_.postCommand(cmd);  // ✅ Replay into engine
    }
}
