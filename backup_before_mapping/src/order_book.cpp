#include "order_book.h"
#include <fstream>
#include <iostream>

// Add new order and try to match
std::vector<Trade> OrderBook::addOrder(const Order& order) {
    std::vector<Trade> trades;
    int remaining_qty = order.quantity;

    // BUY order
    if (order.side == OrderSide::Buy) {
        auto ask_it = asks.begin();
        while (ask_it != asks.end() && remaining_qty > 0 && order.price >= ask_it->first) {
            auto& ask_list = ask_it->second;
            while (!ask_list.empty() && remaining_qty > 0) {
                Order* sell_order = ask_list.front();
                int trade_qty = std::min(remaining_qty, sell_order->quantity);

                // Create trade (match Trade struct: trade_id, buy_id, sell_id, instrument, price, qty)
                trades.push_back({0, order.id, sell_order->id, order.instrument, sell_order->price, trade_qty});

                // Update quantities
                remaining_qty -= trade_qty;
                sell_order->quantity -= trade_qty;

                if (sell_order->quantity == 0) {
                    order_locations.erase(sell_order->id);
                    order_pool.erase(sell_order->id);
                    ask_list.pop_front();
                }
            }
            if (ask_list.empty()) ask_it = asks.erase(ask_it);
            else ++ask_it;
        }

        if (remaining_qty > 0) {
            Order new_order = order;
            new_order.quantity = remaining_qty;
            order_pool[order.id] = new_order;
            Order* stored = &order_pool[order.id];
            bids[order.price].push_back(stored);
            order_locations[order.id] = {stored, --bids[order.price].end()};
        }
    }

    // SELL order
    else if (order.side == OrderSide::Sell) {
        auto bid_it = bids.begin();
        while (bid_it != bids.end() && remaining_qty > 0 && order.price <= bid_it->first) {
            auto& bid_list = bid_it->second;
            while (!bid_list.empty() && remaining_qty > 0) {
                Order* buy_order = bid_list.front();
                int trade_qty = std::min(remaining_qty, buy_order->quantity);

                trades.push_back({0, buy_order->id, order.id, order.instrument, buy_order->price, trade_qty});

                remaining_qty -= trade_qty;
                buy_order->quantity -= trade_qty;

                if (buy_order->quantity == 0) {
                    order_locations.erase(buy_order->id);
                    order_pool.erase(buy_order->id);
                    bid_list.pop_front();
                }
            }
            if (bid_list.empty()) bid_it = bids.erase(bid_it);
            else ++bid_it;
        }

        if (remaining_qty > 0) {
            Order new_order = order;
            new_order.quantity = remaining_qty;
            order_pool[order.id] = new_order;
            Order* stored = &order_pool[order.id];
            asks[order.price].push_back(stored);
            order_locations[order.id] = {stored, --asks[order.price].end()};
        }
    }

    return trades;
}

// Cancel an existing order
void OrderBook::cancelOrder(int orderId) {
    auto it = order_locations.find(orderId);
    if (it != order_locations.end()) {
        Order* ord = it->second.order_ptr;
        double price = ord->price;
        if (ord->side == OrderSide::Buy) {
            auto& list = bids[price];
            list.erase(it->second.list_iterator);
            if (list.empty()) bids.erase(price);
        } else {
            auto& list = asks[price];
            list.erase(it->second.list_iterator);
            if (list.empty()) asks.erase(price);
        }
        order_pool.erase(orderId);
        order_locations.erase(it);
    }
}

// Get current depth snapshot
MarketDepth OrderBook::getMarketDepth() const {
    MarketDepth depth;

    for (const auto& [price, orders] : bids) {
        int qty = 0;
        for (auto* o : orders) qty += o->quantity;
        depth.bids.push_back({price, qty});
    }

    for (const auto& [price, orders] : asks) {
        int qty = 0;
        for (auto* o : orders) qty += o->quantity;
        depth.asks.push_back({price, qty});
    }

    return depth;
}

// Save order book to file
void OrderBook::saveToFile(const std::string& filepath) const {
    std::ofstream out(filepath);
    if (!out.is_open()) return;

    out << "BIDS\n";
    for (const auto& [price, orders] : bids) {
        for (auto* o : orders) {
            out << o->id << "," << price << "," << o->quantity << "\n";
        }
    }

    out << "ASKS\n";
    for (const auto& [price, orders] : asks) {
        for (auto* o : orders) {
            out << o->id << "," << price << "," << o->quantity << "\n";
        }
    }
}

// Load order book from file
void OrderBook::loadFromFile(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) return;

    bids.clear();
    asks.clear();
    order_pool.clear();
    order_locations.clear();

    std::string section;
    while (in >> section) {
        if (section == "BIDS") {
            int id, qty;
            double price;
            char sep;
            while (in >> id >> sep >> price >> sep >> qty) {
                Order ord{id, "UNKNOWN", OrderType::Limit, OrderSide::Buy, price, qty, qty};
                order_pool[id] = ord;
                Order* stored = &order_pool[id];
                bids[price].push_back(stored);
                order_locations[id] = {stored, --bids[price].end()};
            }
        }
        if (section == "ASKS") {
            int id, qty;
            double price;
            char sep;
            while (in >> id >> sep >> price >> sep >> qty) {
                Order ord{id, "UNKNOWN", OrderType::Limit, OrderSide::Sell, price, qty, qty};
                order_pool[id] = ord;
                Order* stored = &order_pool[id];
                asks[price].push_back(stored);
                order_locations[id] = {stored, --asks[price].end()};
            }
        }
    }
}
