#include "matching_engine.h"
#include "logger.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Requirement: QCSIDM_SRS_100 (Benchmark harness for latency)
    constexpr int NUM_ORDERS = 2'000'000; // 2 Million orders

    Logger::log(LogLevel::INFO, "--- ENGINE BENCHMARK ---");
    
    // Create an engine with no listeners to measure raw performance
    MatchingEngine engine(10000);

    Logger::log(LogLevel::INFO, "Pre-loading " + std::to_string(NUM_ORDERS) + " orders into the queue...");

    for (int i = 0; i < NUM_ORDERS; ++i) {
        engine.postCommand({AddOrderCommand{{i, "BENCH", OrderType::Limit, OrderSide::Buy, 100.0, 1}}});
    }
    engine.stop(); // Post the shutdown command at the end

    Logger::log(LogLevel::INFO, "Starting benchmark...");

    auto start_time = std::chrono::high_resolution_clock::now();

    // Run the engine in a separate thread
    std::thread engine_thread(&MatchingEngine::run, &engine);
    // Wait for the engine to process all orders and shut down
    engine_thread.join();

    auto end_time = std::chrono::high_resolution_clock::now();
    Logger::log(LogLevel::INFO, "Benchmark finished.");

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    double duration_s = duration_ms / 1000.0;
    double orders_per_second = NUM_ORDERS / duration_s;
    double latency_ns = (duration_ms * 1'000'000.0) / NUM_ORDERS;

    std::cout << "\n--- BENCHMARK RESULTS ---\n";
    std::cout << "Processed " << NUM_ORDERS << " orders in " << duration_s << " seconds.\n";
    std::cout << "Throughput: " << static_cast<int>(orders_per_second) << " orders/sec\n";
    // Requirement: QCSIDM_SRS_011 (Latency <50µs per order target)
    std::cout << "Avg. Latency: " << static_cast<int>(latency_ns) << " ns/order\n";

    return 0;
}