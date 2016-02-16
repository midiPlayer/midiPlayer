#include "monitorio.h"
#include "websocketserver.h"
#include <QDebug>

MonitorIO::MonitorIO(WebSocketServer *wss): WebSocketServerProvider(wss)
{
    wss->registerProvider(this);
}

void MonitorIO::clientRegistered(QJsonObject msg, int id)
{
}

void MonitorIO::clientUnregistered(QJsonObject msg, int clientIdCounter)
{
}

void MonitorIO::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("error")){
        qDebug() << "MONITOR ERROR: " << msg.value("error");
    }
}

QString MonitorIO::getRequestType()
{
    return "monitor";
}

void MonitorIO::showRessource(QString res, double fadeTime)
{
    QJsonObject msg;
    msg.insert("ressource",res);
    msg.insert("fadeT",fadeTime);
    sendMsg(msg);
}
