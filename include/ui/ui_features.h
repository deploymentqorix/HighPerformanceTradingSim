#pragma once
#include <QMainWindow>
#include <QShortcut>
#include <QSettings>
#include <QDockWidget>
#include <QAction>

// Requirement Mapping
// QCSIDM_SRS_071 - Custom Indicators
// QCSIDM_SRS_072 - Multi-window
// QCSIDM_SRS_073 - Detachable tabs
// QCSIDM_SRS_074 - Theme toggle
// QCSIDM_SRS_075 - Save/Load layout
// QCSIDM_SRS_076 - Hotkeys
// QCSIDM_SRS_077 - Zoom & pan
// QCSIDM_SRS_078 - Refresh toggle
// QCSIDM_SRS_079 - Notifications
// QCSIDM_SRS_080 - Error logs viewer

class UIFeatures : public QObject {
    Q_OBJECT

public:
    explicit UIFeatures(QMainWindow* window);

    void applyDarkTheme();
    void applyLightTheme();
    void saveLayout();
    void loadLayout();

signals:
    void showNotification(const QString& message);

private:
    QMainWindow* mainWindow;
    bool darkMode = true;
};
