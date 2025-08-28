#pragma once
#include <string>
#include <atomic>
#include <chrono>
#include <vector>

// QCSIDM_SRS_101: Benchmark harness for latency
// QCSIDM_SRS_102: Performance metrics output
// QCSIDM_SRS_103: Integration with perf/ASAN (hooks provided)
// QCSIDM_SRS_104: Memory sanitizer / diagnostic build support
// QCSIDM_SRS_105: CI / pipeline friendly output
// QCSIDM_SRS_106: Telemetry logging with levels
// QCSIDM_SRS_107: Engine tick rate monitoring
// QCSIDM_SRS_108: Message throughput counters
// QCSIDM_SRS_109: Profiling hooks for performance analysis
// QCSIDM_SRS_110: Packaging/build scripts (helpers)

struct TelemetrySample {
    std::chrono::steady_clock::time_point ts;
    uint64_t commands_processed;
    uint64_t trades_executed;
};

class Telemetry {
public:
    Telemetry();
    ~Telemetry();

    void recordCommand();
    void recordTrade();
    void tick();                       // called by engine each loop
    void exportMetrics(const std::string& filepath) const;
    TelemetrySample snapshot() const;

    // For CI/bench: produce one-line CSV summary
    std::string csvSummary() const;

private:
    std::atomic<uint64_t> commands_{0};
    std::atomic<uint64_t> trades_{0};
    std::chrono::steady_clock::time_point start_time_;
    std::vector<TelemetrySample> samples_;
};
