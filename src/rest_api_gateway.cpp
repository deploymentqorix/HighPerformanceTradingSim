#include "rest_api_gateway.h"
#include "matching_engine.h"
#include "httplib.h"

// Constructor: Initializes members and sets up the server
RestApiGateway::RestApiGateway(const std::string& host, int port, MatchingEngine& engine)
    : matchingEngine(engine), apiHost(host), apiPort(port) {
    server = std::make_unique<httplib::Server>();
    setupRoutes();
    std::cout << "REST API Gateway initialized on " << host << ":" << port << std::endl;
}

// Destructor
RestApiGateway::~RestApiGateway() {
    stop();
}

// Starts the server to listen for requests
void RestApiGateway::run() {
    std::cout << "REST API Gateway thread started." << std::endl;
    server->listen(apiHost.c_str(), apiPort);
}

// Stops the server
void RestApiGateway::stop() {
    if (server && server->is_running()) {
        server->stop();
    }
}

// Defines the API endpoints (e.g., POST /order)
void RestApiGateway::setupRoutes() {
    server->Get("/ping", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("pong", "text/plain");
    });
}

// --- API method implementations ---
// QCSIDM_SRS_040 - External API for order submission
std::string RestApiGateway::submitOrder(const std::string& orderJson) { return "{}"; }

// QCSIDM_SRS_041 - Cancel/modify orders
std::string RestApiGateway::cancelOrder(const std::string& orderId) { return "{}"; }

// QCSIDM_SRS_042 - Query order status
std::string RestApiGateway::getOrderStatus(const std::string& orderId) { return "{}"; }

// QCSIDM_SRS_043 - Query account/position
std::string RestApiGateway::getAccountInfo(const std::string& accountId) { return "{}"; }

// QCSIDM_SRS_044 - Input validation
bool RestApiGateway::validateJson(const nlohmann::json& j) { return true; }

// QCSIDM_SRS_046 - Error handling
std::string RestApiGateway::makeErrorResponse(const std::string& msg) { return "{\"error\":\"" + msg + "\"}"; }