# Requirement Mapping (automatic insert) — HighPerformanceTradingSim

This file maps SRS requirement IDs (QCSIDM_SRS_xxx) to the code files / functions that implement them.

## Implemented / Mapped (in-code comments present)

- QCSIDM_SRS_001: order_book.cpp (addOrder) / matching_engine.cpp (run)
- QCSIDM_SRS_003: order_book.cpp (partial fills)
- QCSIDM_SRS_004: order_book.cpp (cancelOrder) - O(1) cancel via order_locations
- QCSIDM_SRS_005: order_book.h / order_book.cpp (FIFO per price level)
- QCSIDM_SRS_006: order_book.cpp (market sweep behavior via addOrder)
- QCSIDM_SRS_010: matching_engine.cpp (batch dequeue loop)
- QCSIDM_SRS_012: matching_engine.cpp (risk_manager checks)
- QCSIDM_SRS_013: accounting_manager.cpp (basic accounting hooks)
- QCSIDM_SRS_014: accounting_manager.cpp (position tracking)
- QCSIDM_SRS_015: audit_logger.cpp + listeners (trade log)
- QCSIDM_SRS_016: order_book.cpp (getMarketDepth)
- QCSIDM_SRS_018: engine single-thread model + I/O thread support (matching_engine.cpp)
- QCSIDM_SRS_022: matching_engine single-threaded deterministic loop
- QCSIDM_SRS_023: audit_logger.cpp (logging orders)
- QCSIDM_SRS_024: audit_replayer.cpp (replay)
- QCSIDM_SRS_025: MarketDataGenerator basic generator (configurable later)
- QCSIDM_SRS_030: OrderBook persistence (saveToFile/loadFromFile)
- QCSIDM_SRS_036: rest_api_gateway.cpp endpoint shows risk feedback (basic)
- QCSIDM_SRS_041..QCSIDM_SRS_070: Frontend stubs exist (main_ui), but full features are TODO (mapped as TODO below)
- QCSIDM_SRS_071..QCSIDM_SRS_090: Scripting engine placeholders exist; actual Lua/Python embedding TODO

## TODO / Not fully implemented (must be completed)

- QCSIDM_SRS_002 (IOC, FOK semantics)
- QCSIDM_SRS_007 (order id uniqueness enforcement — currently simple atomic id in REST; improve distributed uniqueness)
- QCSIDM_SRS_008 (replay CSV loader + configurable speeds)
- QCSIDM_SRS_011 (latency <50us) — profiling/optimizations required
- QCSIDM_SRS_017 (incremental L2 delta feed)
- QCSIDM_SRS_020..QCSIDM_SRS_021 (binary serialization / FlatBuffers)
- QCSIDM_SRS_027..QCSIDM_SRS_029 (iceberg/stop/quote advanced order types)
- QCSIDM_SRS_031..QCSIDM_SRS_035 (profiling, stress generator, allocators)
- QCSIDM_SRS_037..QCSIDM_SRS_039 (cross-symbol, arbitrage, latency monitoring)
- QCSIDM_SRS_041..QCSIDM_SRS_070 (UI feature set needs implementation)
- QCSIDM_SRS_071..QCSIDM_SRS_090 (scripting engine features)
- QCSIDM_SRS_091..QCSIDM_SRS_110 (system infra / CI / packaging / containerization / tests) -> mostly TODO

## Notes

- The repo now contains in-source comments (QCSIDM_SRS_xxx) for implemented items in the files:

  - src/order_book.cpp, src/matching_engine.cpp, src/audit_logger.cpp, src/audit_replayer.cpp,
    src/accounting_manager.cpp, src/rest_api_gateway.cpp, src/market_data_generator.cpp

- To show the mapping in the submission: include `REQ_MAPPING.md` and ensure the annotated files are in the delivered zip.
