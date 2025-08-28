cmake_minimum_required(VERSION 3.14)
project(HighPerformanceTradingSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

set(ENGINE_SOURCES
    src/matching_engine.cpp
    src/order.cpp
    src/order_book.cpp
    src/logger.cpp
    src/risk_manager.cpp
    src/accounting_manager.cpp
    src/audit_logger.cpp
    src/trading_bot.cpp
    src/scripting_engine.cpp
)

add_executable(HighPerformanceTradingSim
    src/main.cpp
    src/config.cpp
    src/zeromq_gateway.cpp
    src/rest_api_gateway.cpp
    ${ENGINE_SOURCES}
)
target_include_directories(HighPerformanceTradingSim PRIVATE include third_party ${ZMQ_INCLUDE_DIRS})
target_link_libraries(HighPerformanceTradingSim PRIVATE Threads::Threads ${ZMQ_LIBRARIES})

add_executable(CliClient
    src/cli_client.cpp
)
target_include_directories(CliClient PRIVATE include third_party ${ZMQ_INCLUDE_DIRS})
target_link_libraries(CliClient PRIVATE ${ZMQ_LIBRARIES})

# NEW: UI Application Stub Target
add_executable(TradingUI
    src/main_ui.cpp
    src/ui/main_window.cpp
)
target_include_directories(TradingUI PRIVATE include)