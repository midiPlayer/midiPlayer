#include "websocketconnector.h"
#include <QDebug>
WebSocketConnector::WebSocketConnector(QObject *parent) :QObject(parent)
{
    qDebug() << "wsc crerated";
}

void WebSocketConnector::startConnection(const QString &)
{
    qDebug() << "startConnectionCalled";
}

QString WebSocketConnector::url() const
{
    return "";
}

