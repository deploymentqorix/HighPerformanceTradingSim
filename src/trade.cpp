#include "trade.h"
#include <sstream>

// ===============================
// Trade Implementation
// ===============================

// QCSIDM_SRS_010: System shall generate trade records when orders match
// QCSIDM_SRS_011: Each trade shall include tradeId, buyOrderId, sellOrderId, price, qty, timestamp
// QCSIDM_SRS_012: Trade data shall be immutable after creation
// QCSIDM_SRS_013: Trade shall provide string serialization for logging/reporting

Trade::Trade(std::string tradeId,
             std::string buyOrderId,
             std::string sellOrderId,
             double price,
             int quantity,
             long timestamp)
    : tradeId(std::move(tradeId)),
      buyOrderId(std::move(buyOrderId)),
      sellOrderId(std::move(sellOrderId)),
      price(price),
      quantity(quantity),
      timestamp(timestamp) 
{
    // (requirement: QCSIDM_SRS_012)
    if (price <= 0 || quantity <= 0) {
        throw std::invalid_argument("Invalid trade parameters");
    }
}

std::string Trade::toString() const {
    // (requirement: QCSIDM_SRS_013)
    std::ostringstream oss;
    oss << "Trade[ID=" << tradeId
        << ", BuyOrder=" << buyOrderId
        << ", SellOrder=" << sellOrderId
        << ", Price=" << price
        << ", Qty=" << quantity
        << ", TS=" << timestamp
        << "]";
    return oss.str();
}
