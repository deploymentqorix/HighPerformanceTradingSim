// TradingChart.h
#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include "MarketDataPublisher.h" // For TradeMessage/OrderBookMessage

// Structure to hold OHLC data for a single candlestick
struct Candle {
    qint64 timestamp;
    double open, high, low, close;
    long volume;
};

// Structure for a depth level
struct DepthLevel {
    double price;
    double size;
};

class TradingChart : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit TradingChart(QWidget* parent = nullptr);
    ~TradingChart();

public slots:
    // Slots to receive data from the MarketDataConsumer
    void onTradeReceived(const TradeMessage& trade);
    void onOrderBookUpdated(const OrderBookMessage& book);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    // Data storage for charts
    QVector<Candle> m_candles;
    QVector<DepthLevel> m_bids;
    QVector<DepthLevel> m_asks;

    // OpenGL-specific members
    QOpenGLShaderProgram* m_program;
    QOpenGLBuffer m_vbo;
};