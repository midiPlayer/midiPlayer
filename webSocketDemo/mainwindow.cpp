#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebSockets/QWebSocket>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalSlider->setRange(0,100);
    server = new EchoServer(8888,true,this);
    connect(server,SIGNAL(numClientsChanged(int)),ui->connectedClients,SLOT(display(int)));
    connect(server,SIGNAL(trigger()),ui->checkBox,SLOT(toggle()));
    connect(server,SIGNAL(valueChanged(int)),ui->verticalSlider,SLOT(setValue(int)));
    connect(ui->verticalSlider,SIGNAL(userChangedValue(int)),server,SLOT(sendValue(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}
