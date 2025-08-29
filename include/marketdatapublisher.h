#pragma once

#include <memory>
#include "RingBuffer.h" // Assuming you have a RingBuffer class

// Define your message types
struct TradeMessage {
    // fields: symbol, price, size, time
};

struct OrderBookMessage {
    // fields: symbol, bid_price, bid_size, ask_price, ask_size
};

// Union for message payload
union MessagePayload {
    TradeMessage trade;
    OrderBookMessage book;
};

// Generic message envelope
struct MarketDataMessage {
    enum Type { TRADE, ORDER_BOOK };
    Type type;
    MessagePayload payload;
};

// The publisher class
class MarketDataPublisher {
public:
    MarketDataPublisher(std::shared_ptr<RingBuffer<MarketDataMessage>> buffer);
    void publishTrade(const TradeMessage& trade);
    void publishOrderBookUpdate(const OrderBookMessage& book);

private:
    std::shared_ptr<RingBuffer<MarketDataMessage>> m_buffer;
};