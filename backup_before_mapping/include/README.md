# High Performance Trading Simulator

This project is a high-performance, multi-threaded, multi-instrument financial trading simulator written in modern C++. It features a decoupled, client/server architecture with a core matching engine and multiple gateways for interaction and monitoring.

## Features Implemented

The simulator implements a significant number of features from the project requirements, focusing on a robust backend and system infrastructure.

**Core Engine:**

- **Event-Driven Matching Engine**: Processes orders with price-time priority.
- **Multi-Instrument Support**: Manages separate, independent order books for multiple symbols.
- **Advanced Order Types**: Supports Limit, Market, IOC, FOK, and Iceberg orders.
- **Partial Fills**: Orders can be partially filled.
- **Efficient Cancellations**: O(1) order cancellation.

**System Architecture & Infrastructure:**

- **Multi-Threaded Architecture**: Dedicated thread for the core engine and separate worker threads for I/O gateways.
- **High-Performance IPC**: Uses a lock-free queue for communication between threads.
- **Interactive Terminal UI**: A real-time, text-based client connects via ZeroMQ to submit orders and view live data dashboards.
- **REST API**: A built-in web server provides REST endpoints for monitoring engine status, positions, order book depth, and live performance metrics.
- **Structured Logging & Audit Trail**: Provides timestamped logging and a replayable audit trail of all commands.
- **JSON Configuration**: Engine parameters are loaded from an external `config.json` file.

**Simulation & Analysis:**

- **Live Accounting**: Tracks positions and realized P&L for each instrument in real-time.
- **Pre-Trade Risk Management**: A risk manager checks all incoming orders against configurable rules.
- **Benchmark Harness**: A separate executable was created to measure core engine latency and throughput.
- **Deterministic Replay**: The system includes a tool to replay an `audit.log` to perfectly reconstruct a previous session.

---

## How to Build

The project uses CMake. You will need `pkg-config` and `libzmq3-dev` installed.

```bash
# For Debian/Ubuntu systems
sudo apt-get update
sudo apt-get install pkg-config libzmq3-dev

# Build the project
mkdir build
cd build
cmake ..
make
```
