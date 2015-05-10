#ifndef REMOTEBEAT_H
#define REMOTEBEAT_H

#include "websocketserverprovider.h"
#include "jackprocessor.h"
#include <QObject>
class RemoteBeat :  public QObject, public WebSocketServerProvider
{
Q_OBJECT
public:
    RemoteBeat(WebSocketServer *ws,JackProcessor *jp,QObject * parent = 0);
    void clientRegistered(QJsonObject msg,int clientIdCounter);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
public slots:
    void onOnset();
    void onBeat();
};

#endif // REMOTEBEAT_H
