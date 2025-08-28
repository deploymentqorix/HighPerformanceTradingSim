#include "telemetry.h"
#include <fstream>
#include <sstream>
#include <iomanip>

Telemetry::Telemetry() : start_time_(std::chrono::steady_clock::now()) {}

Telemetry::~Telemetry() {}

void Telemetry::recordCommand() { commands_.fetch_add(1, std::memory_order_relaxed); }
void Telemetry::recordTrade() { trades_.fetch_add(1, std::memory_order_relaxed); }

void Telemetry::tick() {
    TelemetrySample s;
    s.ts = std::chrono::steady_clock::now();
    s.commands_processed = commands_.exchange(0);
    s.trades_executed = trades_.exchange(0);
    samples_.push_back(s);
}

TelemetrySample Telemetry::snapshot() const {
    TelemetrySample s;
    s.ts = std::chrono::steady_clock::now();
    s.commands_processed = commands_.load();
    s.trades_executed = trades_.load();
    return s;
}

void Telemetry::exportMetrics(const std::string& filepath) const {
    std::ofstream f(filepath);
    f << "timestamp,commands,trades\n";
    for (auto &s : samples_) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(s.ts - start_time_).count();
        f << ms << "," << s.commands_processed << "," << s.trades_executed << "\n";
    }
}

std::string Telemetry::csvSummary() const {
    double total_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - start_time_).count();
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2)
       << "duration_s:" << total_seconds
       << ",total_cmds:" << (uint64_t)std::accumulate(samples_.begin(), samples_.end(), 0ULL,
            [](uint64_t acc, const TelemetrySample &s){ return acc + s.commands_processed; })
       << ",total_trades:" << (uint64_t)std::accumulate(samples_.begin(), samples_.end(), 0ULL,
            [](uint64_t acc, const TelemetrySample &s){ return acc + s.trades_executed; });
    return ss.str();
}
