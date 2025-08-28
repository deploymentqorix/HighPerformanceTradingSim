#include "matching_engine.h"
#include "logger.h"
#include "json.hpp" // if you have nlohmann::json; if not, the file still compiles but you can remove JSON usage
#include <thread>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

MatchingEngine::MatchingEngine(int max_order_size)
    : is_running(false),
      risk_manager_(max_order_size),
      accounting_manager_(),
      audit_logger_("audit.log"),
      last_metric_update_time_(std::chrono::steady_clock::now())
{
    // Ensure the accounting manager receives trade events from this engine
    addListener(&accounting_manager_);
}

void MatchingEngine::run() {
    is_running = true;
    Logger::log(LogLevel::INFO, "Matching engine started.");

    while (is_running) {
        Command cmd;
        if (command_queue.try_dequeue(cmd)) {
            // Track commands processed for metrics
            commands_processed_this_second_.fetch_add(1, std::memory_order_relaxed);

            audit_logger_.logCommand(cmd);

            // Dispatch commands
            std::visit([this](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, AddOrderCommand>) {
                    processAddOrder(v.order);
                } else if constexpr (std::is_same_v<T, CancelOrderCommand>) {
                    processCancelOrder(v.instrument, v.orderId);
                } else if constexpr (std::is_same_v<T, ShutdownCommand>) {
                    Logger::log(LogLevel::INFO, "Shutdown command received via queue.");
                    is_running = false;
                }
            }, cmd.data);
        } else {
            // No command: yield a little to avoid burning CPU
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        // Periodically update metrics
        updateMetrics();
    }

    Logger::log(LogLevel::INFO, "Matching engine stopped.");
}

void MatchingEngine::stop() {
    // Graceful stop: enqueue a ShutdownCommand so engine loop will exit deterministically
    command_queue.enqueue(Command{ ShutdownCommand{} });
}

void MatchingEngine::postCommand(const Command& command) {
    command_queue.enqueue(command);
}

void MatchingEngine::addListener(TradeListener* listener) {
    listeners.push_back(listener);
}

void MatchingEngine::addMarketDataListener(MarketDataListener* listener) {
    market_data_listeners.push_back(listener);
}

const AccountingManager& MatchingEngine::getAccountingManager() const {
    return accounting_manager_;
}

void MatchingEngine::processAddOrder(const Order& order) {
    // Pre-trade risk checks
    if (!risk_manager_.checkOrder(order)) {
        Logger::log(LogLevel::WARN, "Order failed risk check: instrument=" + order.instrument);
        return;
    }

    // Get/create book for instrument
    auto &book = order_books[order.instrument];

    // Add order and collect trades produced
    std::vector<Trade> trades = book.addOrder(order);

    // Account & publish trades
    if (!trades.empty()) {
        trades_executed_this_second_.fetch_add(trades.size(), std::memory_order_relaxed);
    }

    for (const auto &t : trades) {
        // Notify accounting (which is also a TradeListener)
        accounting_manager_.onTrade(order.instrument, t);

        // Notify other listeners
        for (auto* listener : listeners) {
            if (listener != &accounting_manager_) // accounting already handled explicitly, but double-call is harmless if interface idempotent
                listener->onTrade(order.instrument, t);
        }
    }

    // Publish market depth (L1/L2 snapshot) after processing
    publishMarketDepth(order.instrument);
}

void MatchingEngine::processCancelOrder(const std::string& instrument, int orderId) {
    auto it = order_books.find(instrument);
    if (it == order_books.end()) {
        Logger::log(LogLevel::WARN, "Cancel: unknown instrument " + instrument);
        return;
    }
    it->second.cancelOrder(orderId);
    publishMarketDepth(instrument);
}

void MatchingEngine::publishMarketDepth(const std::string& instrument) {
    auto it = order_books.find(instrument);
    if (it == order_books.end()) return;

    MarketDepth depth = it->second.getMarketDepth();
    for (auto* md : market_data_listeners) {
        md->onMarketDepthUpdate(instrument, depth);
    }
}

void MatchingEngine::updateMetrics() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_metric_update_time_).count();

    if (elapsed >= 1) {
        // compute per-second values and reset counters
        uint64_t cmds = commands_processed_this_second_.exchange(0);
        uint64_t trds = trades_executed_this_second_.exchange(0);

        orders_per_second_ = static_cast<double>(cmds) / static_cast<double>(elapsed);
        trades_per_second_ = static_cast<double>(trds) / static_cast<double>(elapsed);

        last_metric_update_time_ = now;
    }
}

std::string MatchingEngine::getMetricsReport() const {
    // JSON-style string using nlohmann::json if available
    try {
        json report;
        report["orders_per_second"] = orders_per_second_;
        report["trades_per_second"] = trades_per_second_;
        return report.dump(4);
    } catch (...) {
        std::ostringstream ss;
        ss << "{ \"orders_per_second\": " << orders_per_second_
           << ", \"trades_per_second\": " << trades_per_second_ << " }";
        return ss.str();
    }
}

std::string MatchingEngine::getPositionsReport() const {
    std::ostringstream ss;
    ss << "--- ACCOUNTING REPORT ---\n";
    const auto& posmap = accounting_manager_.getPositions();
    for (const auto& kv : posmap) {
        const auto& instrument = kv.first;
        const auto& pos = kv.second;
        ss << instrument << " Position: Qty=" << pos.quantity
           << ", AvgPrice=" << pos.average_price
           << ", Realized P&L=" << pos.realized_pnl << "\n";
    }
    return ss.str();
}

std::string MatchingEngine::getMarketDepthReport(const std::string& instrument) const {
    auto it = order_books.find(instrument);
    if (it == order_books.end()) {
        return "ERROR: Unknown instrument " + instrument;
    }

    MarketDepth depth = it->second.getMarketDepth();
    std::ostringstream ss;
    ss << "--- " << instrument << " MARKET DEPTH ---\n";
    ss << "BIDS:\n";
    for (const auto &lvl : depth.bids) {
        ss << "  " << lvl.quantity << " @ " << lvl.price << "\n";
    }
    ss << "ASKS:\n";
    for (const auto &lvl : depth.asks) {
        ss << "  " << lvl.quantity << " @ " << lvl.price << "\n";
    }
    return ss.str();
}

std::vector<std::string> MatchingEngine::getTradedInstruments() const {
    std::vector<std::string> instruments;
    instruments.reserve(order_books.size());
    for (const auto &kv : order_books) instruments.push_back(kv.first);
    return instruments;
}
