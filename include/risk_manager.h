#pragma once
#include <string>
#include <vector>
#include "trade.h"
#include "order.h"

// Requirement Mapping
// QCSIDM_SRS_091 - Portfolio exposure
// QCSIDM_SRS_092 - P&L calculation
// QCSIDM_SRS_093 - Order size limits
// QCSIDM_SRS_094 - Exposure limits
// QCSIDM_SRS_095 - Risk alerts
// QCSIDM_SRS_096 - VaR calculation
// QCSIDM_SRS_097 - Stress testing
// QCSIDM_SRS_098 - Risk reporting
// QCSIDM_SRS_099 - Daily summary
// QCSIDM_SRS_100 - Audit logging

class RiskManager {
public:
    RiskManager(double maxOrderSize, double maxExposure);

    bool validateOrder(const Order& order);
    double calculateExposure(const std::vector<Trade>& trades);
    double calculatePnL(const std::vector<Trade>& trades, double marketPrice);
    void generateRiskReport(const std::string& filename);
    void logAudit(const std::string& msg);

    double calculateVaR(const std::vector<Trade>& trades, double confidenceLevel);
    void runStressTest(const std::vector<Trade>& trades);

private:
    double maxOrderSize;
    double maxExposure;
    std::vector<std::string> auditLog;
};
