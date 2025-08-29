#pragma once

#include <QThread>
#include <QObject>
#include <memory>

// Assuming your message structs are in MarketDataPublisher.h or a similar file.
// If not, define them here or in a common header.
struct TradeMessage {
    qint64 time;
    double price;
    double size;
};

struct OrderBookMessage {
    // Define the fields for your order book updates here.
};

union MessagePayload {
    TradeMessage trade;
    OrderBookMessage book;
};

struct MarketDataMessage {
    enum Type { TRADE, ORDER_BOOK };
    Type type;
    MessagePayload payload;
};

// Assuming you have a RingBuffer class
template <typename T>
class RingBuffer {
public:
    bool try_pop(T& value) {
        // Your implementation of pop
        // Return true on success, false on failure (empty buffer)
        return false;
    }
};

class MarketDataConsumer : public QThread {
    Q_OBJECT

public:
    MarketDataConsumer(std::shared_ptr<RingBuffer<MarketDataMessage>> buffer, QObject* parent = nullptr);
    void run() override;

signals:
    void tradeReceived(const TradeMessage& trade);
    void orderBookUpdated(const OrderBookMessage& book);

private:
    std::shared_ptr<RingBuffer<MarketDataMessage>> m_buffer;
};