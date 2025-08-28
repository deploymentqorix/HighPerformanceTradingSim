#include "ui_features.h"
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

UIFeatures::UIFeatures(QMainWindow* window) : mainWindow(window) {
    // --- Hotkeys (SRS_076) ---
    new QShortcut(QKeySequence("Ctrl+D"), mainWindow, [this]() { applyDarkTheme(); });
    new QShortcut(QKeySequence("Ctrl+L"), mainWindow, [this]() { applyLightTheme(); });
    new QShortcut(QKeySequence("Ctrl+S"), mainWindow, [this]() { saveLayout(); });
    new QShortcut(QKeySequence("Ctrl+O"), mainWindow, [this]() { loadLayout(); });

    // --- Notifications (SRS_079) ---
    connect(this, &UIFeatures::showNotification, [this](const QString& msg) {
        QMessageBox::information(mainWindow, "Notification", msg);
    });
}

// --- Theme (SRS_074) ---
void UIFeatures::applyDarkTheme() {
    QFile file(":/qdarkstyle/dark.qss"); 
    if (file.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(file.readAll());
        darkMode = true;
    }
}

void UIFeatures::applyLightTheme() {
    qApp->setStyleSheet("");
    darkMode = false;
}

// --- Save/Load Layout (SRS_075) ---
void UIFeatures::saveLayout() {
    QSettings settings("MyCompany", "TradingSim");
    settings.setValue("geometry", mainWindow->saveGeometry());
    settings.setValue("state", mainWindow->saveState());
    emit showNotification("Layout Saved!");
}

void UIFeatures::loadLayout() {
    QSettings settings("MyCompany", "TradingSim");
    mainWindow->restoreGeometry(settings.value("geometry").toByteArray());
    mainWindow->restoreState(settings.value("state").toByteArray());
    emit showNotification("Layout Restored!");
}
