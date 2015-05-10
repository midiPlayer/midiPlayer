#include "remotebeat.h"
#include "websocketserver.h"

RemoteBeat::RemoteBeat(WebSocketServer *ws, JackProcessor *jp, QObject *parent) : WebSocketServerProvider(ws),QObject(parent)
{
    connect(jp,SIGNAL(beatNotification()),this,SLOT(onBeat()));
    connect(jp,SIGNAL(onsetNotification()),this,SLOT(onOnset()));
    ws->registerProvider(this);
}

void RemoteBeat::clientRegistered(QJsonObject msg, int id)
{
}

void RemoteBeat::clientUnregistered(QJsonObject msg, int id)
{

}

void RemoteBeat::clientMessage(QJsonObject msg, int id)
{

}

QString RemoteBeat::getRequestType()
{
    return "remoteBeat";
}

void RemoteBeat::onOnset()
{
    QJsonObject msg;
    msg.insert("type","onset");
    sendMsg(msg);
}

void RemoteBeat::onBeat()
{
    QJsonObject msg;
    msg.insert("type","beat");
    sendMsg(msg);
}

