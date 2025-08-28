#pragma once
#include "order_book.h"
#include "command.h"
#include "trade_listener.h"
#include "market_data_listener.h"
#include "risk_manager.h"
#include "accounting_manager.h"
#include "audit_logger.h"
#include "concurrentqueue.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <atomic>
#include <chrono>

class MatchingEngine {
public:
    explicit MatchingEngine(int max_order_size);
    void run();
    void stop();
    void postCommand(const Command& command);
    void addListener(TradeListener* listener);
    void addMarketDataListener(MarketDataListener* listener);

    const AccountingManager& getAccountingManager() const;
    std::string getMetricsReport() const;
    std::string getPositionsReport() const;
    std::string getMarketDepthReport(const std::string& instrument) const;
    std::vector<std::string> getTradedInstruments() const;

private:
    void processAddOrder(const Order& order);
    void processCancelOrder(const std::string& instrument, int orderId);
    void publishMarketDepth(const std::string& instrument);
    void updateMetrics();

    moodycamel::ConcurrentQueue<Command> command_queue;
    std::atomic<bool> is_running;
    RiskManager risk_manager_;
    AccountingManager accounting_manager_;
    AuditLogger audit_logger_;
    std::unordered_map<std::string, OrderBook> order_books;
    std::vector<TradeListener*> listeners;
    std::vector<MarketDataListener*> market_data_listeners;
    
    std::atomic<uint64_t> commands_processed_this_second_{0};
    std::atomic<uint64_t> trades_executed_this_second_{0};
    
    double orders_per_second_{0.0};
    double trades_per_second_{0.0};
    std::chrono::steady_clock::time_point last_metric_update_time_;
};
