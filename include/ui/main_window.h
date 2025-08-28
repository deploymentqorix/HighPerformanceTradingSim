#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QChartView>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

// Requirement Mapping
// QCSIDM_SRS_063 - GUI main window
// QCSIDM_SRS_064 - Real-time chart
// QCSIDM_SRS_065 - Order entry panel
// QCSIDM_SRS_066 - Order book display
// QCSIDM_SRS_067 - Trade history
// QCSIDM_SRS_068 - Performance dashboard
// QCSIDM_SRS_069 - Multiple instrument tabs
// QCSIDM_SRS_070 - Alerts/Status messages

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onBuyClicked();
    void onSellClicked();

private:
    QTabWidget* tabs;
    QChartView* priceChart;
    QTableWidget* orderBook;
    QTableWidget* tradeHistory;
    QWidget* performanceDashboard;
    QLabel* statusLabel;

    // Order entry
    QPushButton* buyButton;
    QPushButton* sellButton;
};
