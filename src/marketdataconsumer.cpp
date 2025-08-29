#include "MarketDataConsumer.h"
#include <QDebug>

MarketDataConsumer::MarketDataConsumer(std::shared_ptr<RingBuffer<MarketDataMessage>> buffer, QObject* parent)
    : QThread(parent), m_buffer(buffer) {}

void MarketDataConsumer::run() {
    // This loop runs in the new thread and pulls data from the ring buffer
    while (true) {
        MarketDataMessage msg;
        if (m_buffer->try_pop(msg)) {
            // Process the message and emit the corresponding signal
            if (msg.type == MarketDataMessage::TRADE) {
                emit tradeReceived(msg.payload.trade);
            } else if (msg.type == MarketDataMessage::ORDER_BOOK) {
                emit orderBookUpdated(msg.payload.book);
            }
        } else {
            // Sleep briefly to avoid busy-waiting and consuming too much CPU
            QThread::usleep(100); 
        }
    }
}