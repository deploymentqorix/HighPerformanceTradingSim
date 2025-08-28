#include "zeromq_gateway.h"
#include <iostream>

ZeroMQGateway::ZeroMQGateway(const std::string& pubEp, const std::string& subEp)
    : pubEndpoint(pubEp),
      subEndpoint(subEp),
      context(1),
      publisher(context, ZMQ_PUB),
      subscriber(context, ZMQ_SUB),
      running(false) {}

void ZeroMQGateway::start() {
    publisher.bind(pubEndpoint);     // SRS_055
    subscriber.connect(subEndpoint); // SRS_055
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    running = true;
    subThread = std::thread([this]() { listenLoop(); });
}

void ZeroMQGateway::stop() {
    running = false;
    if (subThread.joinable()) subThread.join();
    publisher.close();
    subscriber.close();
    context.close();
}

void ZeroMQGateway::publishMarketData(const std::string& data) {
    zmq::message_t msg(data.size());
    memcpy(msg.data(), data.c_str(), data.size());
    publisher.send(msg, zmq::send_flags::none); // SRS_053
}

void ZeroMQGateway::sendOrder(const std::string& orderJson) {
    zmq::message_t msg(orderJson.size());
    memcpy(msg.data(), orderJson.c_str(), orderJson.size());
    publisher.send(msg, zmq::send_flags::none); // SRS_052
}

void ZeroMQGateway::onOrderResponse(std::function<void(const std::string&)> cb) {
    orderResponseHandler = cb;
}

void ZeroMQGateway::listenLoop() {
    while (running) {
        zmq::message_t msg;
        try {
            if (subscriber.recv(msg, zmq::recv_flags::none)) {
                std::string resp(static_cast<char*>(msg.data()), msg.size());
                if (orderResponseHandler) {
                    orderResponseHandler(resp); // SRS_051 + SRS_054
                }
            }
        } catch (const zmq::error_t& e) {
            std::cerr << "ZMQ error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1)); // retry (SRS_054)
        }
    }
}
