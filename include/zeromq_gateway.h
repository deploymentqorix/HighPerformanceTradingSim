#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <zmq.hpp>

// Requirement Mapping
// QCSIDM_SRS_051 - ZMQ pub/sub
// QCSIDM_SRS_052 - Order routing
// QCSIDM_SRS_053 - Market data broadcast
// QCSIDM_SRS_054 - Fault tolerance
// QCSIDM_SRS_055 - Configurable endpoints

class ZeroMQGateway {
public:
    ZeroMQGateway(const std::string& pubEndpoint, const std::string& subEndpoint);

    void start();
    void stop();

    void publishMarketData(const std::string& data);     // SRS_053
    void sendOrder(const std::string& orderJson);        // SRS_052

    void onOrderResponse(std::function<void(const std::string&)> cb);

private:
    std::string pubEndpoint;
    std::string subEndpoint;

    zmq::context_t context;
    zmq::socket_t publisher;
    zmq::socket_t subscriber;

    std::atomic<bool> running;
    std::thread subThread;

    std::function<void(const std::string&)> orderResponseHandler;

    void listenLoop();  // SRS_051 + SRS_054
};
