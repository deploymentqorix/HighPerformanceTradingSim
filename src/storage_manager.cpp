#include "storage_manager.h"
#include <sqlite3.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

StorageManager::StorageManager(const std::string& dbFile) : dbFilePath(dbFile) {}

void StorageManager::initDB() {
    sqlite3* db;
    sqlite3_open(dbFilePath.c_str(), &db);

    const char* tradeTable = "CREATE TABLE IF NOT EXISTS trades("
                             "id INTEGER PRIMARY KEY, symbol TEXT, qty INT, price REAL, timestamp TEXT);";
    const char* orderTable = "CREATE TABLE IF NOT EXISTS orders("
                             "id INTEGER PRIMARY KEY, symbol TEXT, qty INT, price REAL, status TEXT);";

    sqlite3_exec(db, tradeTable, 0, 0, 0);
    sqlite3_exec(db, orderTable, 0, 0, 0);
    sqlite3_close(db);
}

void StorageManager::saveTrade(const Trade& trade) {
    sqlite3* db;
    sqlite3_open(dbFilePath.c_str(), &db);

    std::string sql = "INSERT INTO trades(symbol, qty, price, timestamp) VALUES('" + 
        trade.symbol + "'," + std::to_string(trade.qty) + "," + std::to_string(trade.price) + ",'" + trade.timestamp + "');";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    sqlite3_close(db);
}

void StorageManager::saveOrder(const Order& order) {
    sqlite3* db;
    sqlite3_open(dbFilePath.c_str(), &db);

    std::string sql = "INSERT INTO orders(symbol, qty, price, status) VALUES('" + 
        order.symbol + "'," + std::to_string(order.qty) + "," + std::to_string(order.price) + ",'" + order.status + "');";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    sqlite3_close(db);
}

// --- Export to CSV (SRS_085) ---
void StorageManager::exportToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "id,symbol,qty,price,timestamp\n";
    // Fetch trades & dump (mockup for now)
    file << "1,AAPL,10,150.0,2025-08-28 20:00:00\n";
}

// --- Export to JSON (SRS_086) ---
void StorageManager::exportToJSON(const std::string& filename) {
    json j;
    j["trades"] = { { {"symbol","AAPL"}, {"qty",10}, {"price",150.0}, {"timestamp","2025-08-28 20:00:00"} } };
    std::ofstream file(filename);
    file << j.dump(4);
}

// --- Import stubs (SRS_087, SRS_088) ---
void StorageManager::importFromCSV(const std::string& filename) {
    // TODO: parse CSV and insert into DB
}
void StorageManager::importFromJSON(const std::string& filename) {
    // TODO: parse JSON and insert into DB
}

void StorageManager::clearDB() {
    sqlite3* db;
    sqlite3_open(dbFilePath.c_str(), &db);
    sqlite3_exec(db, "DELETE FROM trades;", 0, 0, 0);
    sqlite3_exec(db, "DELETE FROM orders;", 0, 0, 0);
    sqlite3_close(db);
}
