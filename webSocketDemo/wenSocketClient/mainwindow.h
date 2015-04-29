#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QWebSocket>
#include <QString>

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
    void connectToServer();
    void connected();
    void onTextMessage(QString message);
    void disconnected();
    void sendTrigger();


private:
    Ui::MainWindow *ui;    
     QWebSocket m_webSocket;
};

#endif // MAINWINDOW_H
