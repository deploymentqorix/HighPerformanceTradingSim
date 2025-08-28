#include "audit_replayer.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

/*
  AuditReplayer mapping:
  - QCSIDM_SRS_024: replay audit log into engine (deterministic replay)
  - QCSIDM_SRS_009: replay historical market data (reuse audit for orders)
*/

using json = nlohmann::json;

AuditReplayer::AuditReplayer(const std::string& filepath, MatchingEngine& engine)
    : filepath_(filepath), engine_(engine) {}

void AuditReplayer::run() {
    std::ifstream in(filepath_);
    if (!in.is_open()) throw std::runtime_error("Cannot open audit file");
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        json j = json::parse(line);
        std::string type = j.value("type", "");
        if (type == "ADD") {
            Order o;
            o.id = j["order"]["id"];
            o.instrument = j["order"]["instrument"];
            o.side = (j["order"]["side"] == "BUY") ? OrderSide::Buy : OrderSide::Sell;
            o.price = j["order"]["price"];
            o.quantity = j["order"]["quantity"];
            engine_.postCommand(Command{AddOrderCommand{o}});
        } else if (type == "CANCEL") {
            int id = j["orderId"];
            std::string inst = j["instrument"];
            engine_.postCommand(Command{CancelOrderCommand{id, inst}});
        }
    }
}
