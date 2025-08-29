#pragma once

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    QChartView *candlestickView;
};
