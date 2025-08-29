cmake_minimum_required(VERSION 3.14)
project(HighPerformanceTradingSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Qt setup ---
set(CMAKE_AUTOMOC ON)  # Qt MOC (signals/slots)
set(CMAKE_AUTOUIC ON)  # Optional: Qt Designer .ui files support
set(CMAKE_AUTORCC ON)  # Optional: Qt resource files support

find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)

# --- External Dependencies ---
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# --- Main Backend Engine ---
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
    # src/rest_api_gateway.cpp # Uncomment if needed
    ${ENGINE_SOURCES}
)
target_include_directories(HighPerformanceTradingSim
    PRIVATE include third_party ${ZMQ_INCLUDE_DIRS}
)
target_link_libraries(HighPerformanceTradingSim
    PRIVATE Threads::Threads ${ZMQ_LIBRARIES}
)

# --- CLI Client ---
add_executable(CliClient
    src/cli_client.cpp
)
target_include_directories(CliClient PRIVATE include third_party ${ZMQ_INCLUDE_DIRS})
target_link_libraries(CliClient PRIVATE ${ZMQ_LIBRARIES})

# --- UI Application ---
add_executable(TradingUI
    src/main_ui.cpp
    src/ui/main_window.cpp
)
target_include_directories(TradingUI
    PRIVATE include ${ZMQ_INCLUDE_DIRS}
)
target_link_libraries(TradingUI
    PRIVATE Qt5::Widgets Qt5::Charts ${ZMQ_LIBRARIES}
)

# --- Backend for Testing the UI ---
add_executable(BackendTest
    src/main_backend_test.cpp
)
target_include_directories(BackendTest PRIVATE include ${ZMQ_INCLUDE_DIRS})
target_link_libraries(BackendTest PRIVATE ${ZMQ_LIBRARIES})
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua5.3)

target_include_directories(HighPerformanceTradingSim PRIVATE ${LUA_INCLUDE_DIRS})
target_link_libraries(HighPerformanceTradingSim PRIVATE ${LUA_LIBRARIES})
