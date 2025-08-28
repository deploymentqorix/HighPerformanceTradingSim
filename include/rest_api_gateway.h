#pragma once
#include <string>
#include <map>
#include <nlohmann/json.hpp>

// Requirement Mapping
// QCSIDM_SRS_040 - External API for order submission
// QCSIDM_SRS_041 - Cancel/modify orders
// QCSIDM_SRS_042 - Query order status
// QCSIDM_SRS_043 - Query account/position
// QCSIDM_SRS_044 - Input validation
// QCSIDM_SRS_045 - JSON structured responses
// QCSIDM_SRS_046 - Error handling

class RestApiGateway {
public:
    RestApiGateway();

    std::string submitOrder(const std::string& orderJson);     // SRS_040
    std::string cancelOrder(const std::string& orderId);       // SRS_041
    std::string getOrderStatus(const std::string& orderId);    // SRS_042
    std::string getAccountInfo(const std::string& accountId);  // SRS_043

private:
    bool validateJson(const nlohmann::json& j); // SRS_044
    std::string makeErrorResponse(const std::string& msg); // SRS_046
};
