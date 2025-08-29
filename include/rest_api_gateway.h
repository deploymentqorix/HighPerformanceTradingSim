#pragma once
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <memory> // ADDED: for std::unique_ptr

// Forward-declare the MatchingEngine and the HTTP server class
class MatchingEngine;
namespace httplib {
    class Server;
}

// Requirement Mapping
// ... (your existing mappings)

class RestApiGateway {
public:
    RestApiGateway(const std::string& host, int port, MatchingEngine& engine);
    ~RestApiGateway(); // ADDED: Destructor for proper cleanup

    void run();  // ADDED: To start the web server
    void stop(); // ADDED: To stop the web server

    // --- Existing API methods ---
    std::string submitOrder(const std::string& orderJson);
    std::string cancelOrder(const std::string& orderId);
    std::string getOrderStatus(const std::string& orderId);
    std::string getAccountInfo(const std::string& accountId);

private:
    void setupRoutes(); // ADDED: Helper to define API endpoints like /order

    bool validateJson(const nlohmann::json& j);
    std::string makeErrorResponse(const std::string& msg);
    
    // --- Private Members ---
    MatchingEngine& matchingEngine; // ADDED: Reference to the engine
    std::unique_ptr<httplib::Server> server; // ADDED: The HTTP server object
    std::string apiHost; // ADDED
    int apiPort; // ADDED
};