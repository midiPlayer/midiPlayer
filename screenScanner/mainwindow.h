#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "screenanalyzer.h"
#include "../webSocketConnector/websocketconnector.h"
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void connectPressed();
    void connectionSucceded();
    void connectionFailed();
    void onMessage(QJsonObject msg);
    void sendColors(QList<QColor> colors);

private:
    Ui::MainWindow *ui;
    ScreenAnalyzer analyzer;
    WebSocketConnector ws;
    QSettings settings;
};

#endif // MAINWINDOW_H
