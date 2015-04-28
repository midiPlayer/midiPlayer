#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "echoserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    EchoServer* server;
};

#endif // MAINWINDOW_H
