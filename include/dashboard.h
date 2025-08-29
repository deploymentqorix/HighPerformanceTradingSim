#include "MarketDataConsumer.h"
#include <memory>
// ... other includes

class Dashboard : public QMainWindow {
    Q_OBJECT

public:
    explicit Dashboard(QWidget* parent = nullptr);
    ~Dashboard();

private:
    std::shared_ptr<RingBuffer<MarketDataMessage>> m_marketDataBuffer;
    MarketDataConsumer* m_consumer;

    // UI elements
    QTableWidget* m_tradesTape;
    // ... other widgets
};