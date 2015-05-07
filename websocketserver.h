#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include "websocketserverprovider.h"
#include <QString>
#include <QMap>
#include <QJsonObject>
#include <QObject>
#include <QJsonDocument>
#include <QDebug>
#include <QWebSocket>
#include <QWebSocketServer>

class WebSocketServer : QObject
{
Q_OBJECT
public:
    WebSocketServer(QObject *parent);
    void registerProvider(WebSocketServerProvider *me);
    void unregisterProvider(WebSocketServerProvider *me);
    void sendData(QJsonObject data, QWebSocket *reciever, WebSocketServerProvider *provider);
public slots:
    void onNewConnection();
    void onTextMessage(QString message);
    void onConnectionClosed();

private:
    QList<WebSocketServerProvider*> provider;
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};

#endif // WEBSOCKETSERVER_H
