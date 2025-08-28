#include "rest_api_gateway.h"
#include <iostream>

using json = nlohmann::json;

RestApiGateway::RestApiGateway() {}

std::string RestApiGateway::submitOrder(const std::string& orderJson) {
    try {
        json j = json::parse(orderJson);
        if (!validateJson(j)) return makeErrorResponse("Invalid order format"); // SRS_044

        // TODO: Forward to matching engine
        json resp;
        resp["status"] = "accepted";
        resp["orderId"] = "ORD123";
        return resp.dump(); // SRS_045

    } catch (...) {
        return makeErrorResponse("Failed to parse order JSON");
    }
}

std::string RestApiGateway::cancelOrder(const std::string& orderId) {
    // TODO: connect to order book/matching engine
    json resp;
    resp["status"] = "cancelled";
    resp["orderId"] = orderId;
    return resp.dump(); // SRS_041, 045
}

std::string RestApiGateway::getOrderStatus(const std::string& orderId) {
    json resp;
    resp["orderId"] = orderId;
    resp["status"] = "filled"; // Mock
    return resp.dump(); // SRS_042
}

std::string RestApiGateway::getAccountInfo(const std::string& accountId) {
    json resp;
    resp["accountId"] = accountId;
    resp["balance"] = 100000;
    resp["positions"] = {{"AAPL", 50}, {"TSLA", 20}};
    return resp.dump(); // SRS_043
}

bool RestApiGateway::validateJson(const json& j) {
    return j.contains("symbol") && j.contains("quantity") && j.contains("price");
}

std::string RestApiGateway::makeErrorResponse(const std::string& msg) {
    json resp;
    resp["error"] = msg;
    return resp.dump(); // SRS_046
}
