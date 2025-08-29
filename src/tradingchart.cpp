#include "TradingChart.h"
#include <QDateTime>
#include <QDebug>
#include <QOpenGLShaderProgram>

TradingChart::TradingChart(QWidget* parent) : QOpenGLWidget(parent) {
    // This is a simple constructor
}

void TradingChart::initializeGL() {
    initializeOpenGLFunctions();
    // We'll add shader code here later
    // This part is for setting up your graphics card
}

void TradingChart::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    // This part tells OpenGL how to draw when the window size changes
}

void TradingChart::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // This part clears the screen and is where we will add drawing commands
}

// This is the most important part for you now
void TradingChart::onTradeReceived(const TradeMessage& trade) {
    // This slot receives data from your MarketDataConsumer
    // Let's use it to build our candlestick data

    // Check if we need to start a new candle (e.g., every minute)
    qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();
    qint64 interval = 60000; // 1 minute in milliseconds

    if (m_lastCandleTimestamp == 0 || (currentTimestamp - m_lastCandleTimestamp) > interval) {
        // We're starting a new candle
        if (m_lastCandleTimestamp != 0) {
            // Push the old candle to our list
            m_candles.append(m_currentCandle);
        }
        m_currentCandle.timestamp = currentTimestamp;
        m_currentCandle.open = trade.price;
        m_currentCandle.high = trade.price;
        m_currentCandle.low = trade.price;
        m_currentCandle.close = trade.price;
        m_currentCandle.volume = trade.size;
        m_lastCandleTimestamp = currentTimestamp;
    } else {
        // Update the current candle with new trade data
        m_currentCandle.close = trade.price;
        m_currentCandle.high = std::max(m_currentCandle.high, trade.price);
        m_currentCandle.low = std::min(m_currentCandle.low, trade.price);
        m_currentCandle.volume += trade.size;
    }

    // After updating the data, tell the widget to redraw itself
    update();
}