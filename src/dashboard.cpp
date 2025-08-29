#include "Dashboard.h"
#include "TradingChart.h" // Assuming you have this file

Dashboard::Dashboard(QWidget* parent)
    : QMainWindow(parent) {
    // Step 1: Create the ring buffer and consumer
    m_marketDataBuffer = std::make_shared<RingBuffer<MarketDataMessage>>();
    // You need to pass this buffer to your backend's MarketDataPublisher as well.
    m_consumer = new MarketDataConsumer(m_marketDataBuffer, this);
    m_consumer->start(); // Start the consumer thread

    // Step 2: Create UI elements
    m_tradesTape = new QTableWidget(this);
    TradingChart* chart = new TradingChart(this);

    // Step 3: Connect signals to slots to update the UI
    connect(m_consumer, &MarketDataConsumer::tradeReceived, this, &Dashboard::onTradeReceived);
    connect(m_consumer, &MarketDataConsumer::orderBookUpdated, this, &Dashboard::onOrderBookUpdated);
    connect(m_consumer, &MarketDataConsumer::tradeReceived, chart, &TradingChart::onTradeReceived);
    // ... connect other signals

    // Step 4: Arrange your UI elements
    // ...
}