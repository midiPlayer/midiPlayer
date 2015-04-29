#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectBtn,SIGNAL(clicked()),this,SLOT(connectToServer()));
    connect(&m_webSocket,SIGNAL(connected()),this,SLOT(connected()));
    connect(&m_webSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(ui->triggerBtn,SIGNAL(clicked()),this,SLOT(sendTrigger()));
}

MainWindow::~MainWindow()
{
    m_webSocket.disconnect();
    delete ui;
}

void MainWindow::connectToServer()
{
    m_webSocket.open(QUrl(ui->urlEdit->text()));
    ui->connectBtn->setEnabled(false);
}

void MainWindow::connected()
{
    connect(&m_webSocket,SIGNAL(textMessageReceived(QString)),this,SLOT(onTextMessage(QString)));
    ui->triggerBtn->setEnabled(true);
}

void MainWindow::onTextMessage(QString message)
{
    qDebug() << message;
}

void MainWindow::disconnected()
{
    ui->connectBtn->setEnabled(true);
    ui->triggerBtn->setEnabled(false);
}

void MainWindow::sendTrigger()
{
    m_webSocket.sendTextMessage("trigger");
}
