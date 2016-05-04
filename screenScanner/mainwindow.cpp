#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ws(this),
    ui(new Ui::MainWindow)
{

    qRegisterMetaType<QList<QColor> >("colorlist");
    ui->setupUi(this);
    ui->addressEdit->setText(settings.value("url").toString());
    connect(ui->connectBtn,SIGNAL(clicked(bool)),this,SLOT(connectPressed()));
    connect(&ws,SIGNAL(connectionSucceded()),this,SLOT(connectionSucceded()));
    connect(&ws,SIGNAL(connectionFailed()),this,SLOT(connectionFailed()));
    connect(&ws,SIGNAL(message(QJsonObject)),this,SLOT(onMessage(QJsonObject)));
    connect(&analyzer,SIGNAL(color(QList<QColor>)),this,SLOT(sendColors(QList<QColor>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectPressed()
{
    if(ws.isConnected()){
        ws.startConnection("");
        connectionFailed();
    }
    else{
        ws.startConnection(ui->addressEdit->text());
        ui->connectBtn->setText("Connecting");
        ui->connectBtn->setEnabled(false);
        ui->addressEdit->setEnabled(false);
    }
}

void MainWindow::connectionSucceded()
{
    ui->connectBtn->setText("Discconnect");
    ui->connectBtn->setEnabled(true);
    ws.registerForRequestType("screenScanner");
    analyzer.start();
    settings.setValue("url",ui->addressEdit->text());
}

void MainWindow::connectionFailed()
{
    analyzer.stop();
    ui->connectBtn->setText("Connect");
    ui->connectBtn->setEnabled(true);
    ui->connectBtn->setEnabled(true);
}

void MainWindow::onMessage(QJsonObject msg)
{
    qDebug() << msg;
}

void MainWindow::sendColors(QList<QColor> colors)
{
    QJsonArray colorsJson;
    foreach (QColor color, colors) {
        colorsJson.append(color.name());
    }
    QJsonObject obj;
    obj.insert("colors",colorsJson);
    QJsonDocument d;
    d.setObject(obj);
    ws.sendMessge(d.toJson());
}
