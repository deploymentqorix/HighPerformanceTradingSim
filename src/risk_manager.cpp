#include "risk_manager.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

RiskManager::RiskManager(double maxOrder, double maxExp) : maxOrderSize(maxOrder), maxExposure(maxExp) {}

bool RiskManager::validateOrder(const Order& order) {
    if (order.qty > maxOrderSize) {
        std::cerr << "[Risk Alert] Order size exceeds limit!\n";
        logAudit("Order rejected: size exceeds limit.");
        return false;
    }
    return true;
}

double RiskManager::calculateExposure(const std::vector<Trade>& trades) {
    double exposure = 0.0;
    for (const auto& t : trades) {
        exposure += t.qty * t.price;
    }
    if (exposure > maxExposure) {
        std::cerr << "[Risk Alert] Portfolio exposure exceeds limit!\n";
        logAudit("Exposure exceeded.");
    }
    return exposure;
}

double RiskManager::calculatePnL(const std::vector<Trade>& trades, double marketPrice) {
    double pnl = 0.0;
    for (const auto& t : trades) {
        pnl += (marketPrice - t.price) * t.qty;
    }
    return pnl;
}

void RiskManager::generateRiskReport(const std::string& filename) {
    json report;
    report["alerts"] = auditLog;
    std::ofstream file(filename);
    file << report.dump(4);
}

void RiskManager::logAudit(const std::string& msg) {
    auditLog.push_back(msg);
}

double RiskManager::calculateVaR(const std::vector<Trade>& trades, double confidenceLevel) {
    // Simplified VaR = exposure * Z-score * volatility
    double exposure = calculateExposure(trades);
    double z = (confidenceLevel == 0.95) ? 1.65 : 2.33;
    double volatility = 0.02; // placeholder
    return exposure * z * volatility;
}

void RiskManager::runStressTest(const std::vector<Trade>& trades) {
    double exposure = calculateExposure(trades);
    double stressedLoss = exposure * 0.3; // assume 30% market crash
    std::cerr << "[Stress Test] Exposure " << exposure << " → Potential Loss " << stressedLoss << "\n";
    logAudit("Stress test run: potential loss " + std::to_string(stressedLoss));
}
