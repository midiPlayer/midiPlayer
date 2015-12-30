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

class WebSocketServer : public QObject
{
Q_OBJECT

public:
    WebSocketServer(QObject *parent);
    void registerProvider(WebSocketServerProvider *me);
    void unregisterProvider(WebSocketServerProvider *me);
    void sendData(QJsonObject data, QWebSocket *reciever, WebSocketServerProvider *provider,bool onlyToProviderId = 0);

    void disconnectAllClients();
public slots:
    void onNewConnection();
    void onTextMessage(QString message);
    void onConnectionClosed();

signals:
    void clientClosed();

private:
    QList<WebSocketServerProvider*> provider;
    int providerIdCounter;
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};

#endif // WEBSOCKETSERVER_H

