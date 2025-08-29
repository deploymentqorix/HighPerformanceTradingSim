#include "ui/main_window.h"
#include <QtCharts/QChart>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *chart = new QtCharts::QChart();
    candlestickView = new QtCharts::QChartView(chart, this);
    setCentralWidget(candlestickView);
}

MainWindow::~MainWindow() {}
