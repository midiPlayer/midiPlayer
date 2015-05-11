#ifndef WEBSOCKETSERVERPROVIDER_H
#define WEBSOCKETSERVERPROVIDER_H

#include <QList>
#include <QString>
#include <QJsonObject>
#include <QWebSocket>
class WebSocketServer;

class WebSocketServerProvider
{
public:
    WebSocketServerProvider(WebSocketServer* s);
    virtual QString getRequestType() = 0;
    void registerClient(QJsonObject msg, QWebSocket *client);
    void unregisterClient(QJsonObject msg, QWebSocket *client);
    void unregisterClient(QWebSocket *client);
    void onMessage(QJsonObject msg, QWebSocket *client);
    int providerId;
private:
    virtual void clientRegistered(QJsonObject msg,int clientIdCounter) = 0;
    virtual void clientUnregistered(QJsonObject msg,int clientIdCounter) = 0;
    virtual void clientMessage(QJsonObject msg,int clientIdCounter) = 0;

protected:
    bool sendMsg(QJsonObject msg, int client_id, bool onlyForProvierID = false);
    bool sendMsg(QJsonObject msg, bool onlyForProvierID = false);
    bool sendMsgButNotTo(QJsonObject msg, int client_id, bool onlyForProvierID = false);

    int clientIdCounter;
    QMap<QWebSocket*,int> connectedSockets;
    WebSocketServer *server;
};

#endif // WEBSOCKETSERVERPROVIDER_H
