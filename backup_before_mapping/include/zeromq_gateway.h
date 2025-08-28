#pragma once
#include <atomic>
#include <string>

class MatchingEngine;

class ZeroMQGateway {
public:
    ZeroMQGateway(const std::string& address, MatchingEngine& engine);
    void run();
    void stop();

private:
    std::string address_;
    MatchingEngine& engine_;
    std::atomic<bool> is_running_;
};
