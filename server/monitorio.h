#ifndef MONITORIO_H
#define MONITORIO_H

#include "websocketserverprovider.h"

class MonitorIO : public WebSocketServerProvider
{
public:
    MonitorIO(WebSocketServer *wss);
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
    void showRessource(QString res, double fadeTime);
};

#endif // MONITORIO_H
