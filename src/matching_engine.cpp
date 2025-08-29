#include "MatchingEngine.h"

MatchingEngine::MatchingEngine(std::shared_ptr<MarketDataPublisher> publisher)
    : m_publisher(publisher) {
    // ...
}

void MatchingEngine::onNewOrder(const Order& order) {
    // Matching logic
    // If a trade occurs:
    // ...
    TradeMessage trade;
    // populate trade fields from matched order/book
    m_publisher->publishTrade(trade);

    // After a match, or if a resting order is placed, update the book:
    // ...
    OrderBookMessage bookUpdate;
    // populate bookUpdate fields from m_orderBook state
    m_publisher->publishOrderBookUpdate(bookUpdate);
}