#include "main_window.h"
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>

using namespace QtCharts;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) 
{
    tabs = new QTabWidget(this);

    // --- Price Chart (SRS_064) ---
    QLineSeries* series = new QLineSeries();
    series->append(0, 100);
    series->append(1, 101);

    QChart* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    priceChart = new QChartView(chart);

    // --- Order Book (SRS_066) ---
    orderBook = new QTableWidget(10, 3);
    orderBook->setHorizontalHeaderLabels({"Price", "Qty", "Side"});

    // --- Trade History (SRS_067) ---
    tradeHistory = new QTableWidget(10, 3);
    tradeHistory->setHorizontalHeaderLabels({"Time", "Price", "Qty"});

    // --- Performance Dashboard (SRS_068) ---
    performanceDashboard = new QWidget();
    QVBoxLayout* perfLayout = new QVBoxLayout();
    perfLayout->addWidget(new QLabel("PnL: $0"));
    perfLayout->addWidget(new QLabel("Latency: 0.2ms"));
    performanceDashboard->setLayout(perfLayout);

    // --- Order Entry (SRS_065) ---
    QWidget* orderEntry = new QWidget();
    QFormLayout* formLayout = new QFormLayout();

    QSpinBox* qtyInput = new QSpinBox();
    qtyInput->setRange(1, 10000);

    QDoubleSpinBox* priceInput = new QDoubleSpinBox();
    priceInput->setRange(1, 100000);
    priceInput->setDecimals(2);

    buyButton = new QPushButton("Buy");
    sellButton = new QPushButton("Sell");
    connect(buyButton, &QPushButton::clicked, this, &MainWindow::onBuyClicked);
    connect(sellButton, &QPushButton::clicked, this, &MainWindow::onSellClicked);

    formLayout->addRow("Quantity", qtyInput);
    formLayout->addRow("Price", priceInput);
    formLayout->addRow(buyButton, sellButton);

    orderEntry->setLayout(formLayout);

    // --- Alerts/Status (SRS_070) ---
    statusLabel = new QLabel("Connected to Exchange");

    // --- Tabs (SRS_069) ---
    tabs->addTab(priceChart, "Chart");
    tabs->addTab(orderBook, "Order Book");
    tabs->addTab(tradeHistory, "Trades");
    tabs->addTab(performanceDashboard, "Performance");
    tabs->addTab(orderEntry, "Order Entry");

    setCentralWidget(tabs);
    statusBar()->addWidget(statusLabel);
}

void MainWindow::onBuyClicked() {
    statusLabel->setText("Order Sent: BUY");
}

void MainWindow::onSellClicked() {
    statusLabel->setText("Order Sent: SELL");
}
