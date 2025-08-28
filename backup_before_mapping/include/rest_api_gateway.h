#pragma once
#include <string>
#include <memory>
#include <thread>

class MatchingEngine;
namespace httplib { class Server; }

class RestApiGateway {
public:
    RestApiGateway(const std::string& host, int port, MatchingEngine& engine);
    ~RestApiGateway();
    void run();
    void stop();

private:
    std::string host_;
    int port_;
    MatchingEngine& engine_;
    std::unique_ptr<httplib::Server> server_;
    std::thread server_thread_;
};
