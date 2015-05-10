#include "websocketconnector.h"
#include <QDebug>
WebSocketConnector::WebSocketConnector(QObject* parent) :QObject(parent),webInterface(WebInterface::getInstance()),requestType(""),urlV("")
{
}

WebSocketConnector::~WebSocketConnector()
{
    webInterface.unregisterConnector(this);
}


void WebSocketConnector::startConnection(const QString &url)
{
    qDebug() << "startConnectionCalled";
    webInterface.connectToServer(url);
    connect(&webInterface,SIGNAL(connectedS()),this,SLOT(connected()));
    connect(&webInterface,SIGNAL(disconnectedS()),this,SLOT(disconnected()));
    urlV = url;
}

QString WebSocketConnector::url() const
{
    return urlV;
}

QString WebSocketConnector::empty() const
{
    return "";
}

QString WebSocketConnector::getRequestType()
{
    return requestType;
}

void WebSocketConnector::registerForRequestType(QString type)
{
    requestType = type;
    webInterface.registerConnector(this);
}

void WebSocketConnector::onMessage(QJsonObject msg)
{
    emit message(msg);
}

void WebSocketConnector::sendMessge(QString msg)
{
    QJsonDocument d = QJsonDocument::fromJson(msg.toUtf8());
    webInterface.sendMsg(this,d.object());
}

void WebSocketConnector::connected()
{
    emit connectionSucceded();
}

void WebSocketConnector::disconnected()
{
    emit connectionFailed();
}

