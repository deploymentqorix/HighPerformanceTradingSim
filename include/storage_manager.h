#pragma once
#include <string>
#include <vector>
#include "trade.h"
#include "order.h"

// Requirement Mapping
// QCSIDM_SRS_081 - Store trades
// QCSIDM_SRS_082 - Store orders
// QCSIDM_SRS_083 - Persist portfolio
// QCSIDM_SRS_084 - Query trades/orders
// QCSIDM_SRS_085 - Export CSV
// QCSIDM_SRS_086 - Export JSON
// QCSIDM_SRS_087 - Import CSV
// QCSIDM_SRS_088 - Import JSON
// QCSIDM_SRS_089 - Clear DB
// QCSIDM_SRS_090 - DB Config

class StorageManager {
public:
    StorageManager(const std::string& dbFile);

    void initDB();
    void saveTrade(const Trade& trade);
    void saveOrder(const Order& order);
    std::vector<Trade> loadTrades();
    std::vector<Order> loadOrders();

    void exportToCSV(const std::string& filename);
    void exportToJSON(const std::string& filename);
    void importFromCSV(const std::string& filename);
    void importFromJSON(const std::string& filename);

    void clearDB();

private:
    std::string dbFilePath;
};
