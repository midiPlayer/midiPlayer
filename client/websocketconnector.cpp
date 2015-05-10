#include "websocketconnector.h"
#include <QDebug>
WebSocketConnector::WebSocketConnector(QObject* parent) :QObject(parent),webInterface(WebInterface::getInstance()),requestType()
{

}


void WebSocketConnector::startConnection(const QString &)
{
    qDebug() << "startConnectionCalled";
}

QString WebSocketConnector::url() const
{
    return "";
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
    emit onMessage(msg);
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

