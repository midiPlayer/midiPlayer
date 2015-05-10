#include "webinterface.h"
#include "websocketconnector.h"

WebInterface::WebInterface(QObject *parent):QObject(parent), connectors(),ws(),isConnectedP(false)
{
}


WebInterface &WebInterface::getInstance()
{
    static WebInterface w;
    return w;
}

void WebInterface::registerWsConnector(WebSocketConnector *wsc)
{
    if(!connectors.contains(wsc))
        connectors.append(wsc);
}

bool WebInterface::connectToServer(QString url)
{
    connect(&ws,SIGNAL(connected()),this,SLOT(connected()));
    connect(&ws,SIGNAL(disconnected()),this,SLOT(disconnected()));
    ws.open(QUrl(url));
    return true;
}

bool WebInterface::isConnected()
{
    return isConnectedP;
}

void WebInterface::registerConnector(WebSocketConnector *connector)
{
    connectors.append(connector);
    QJsonObject msg;
    msg.insert("register",connector->getRequestType());
    QJsonDocument d;
    d.setObject(msg);
    ws.sendTextMessage(d.toJson());
}

void WebInterface::sendMsg(WebSocketConnector *connector, QJsonObject msg)
{
    QJsonObject send;
    send.insert("provider",connector->getRequestType());
    send.insert("data",msg);
    QJsonDocument d;
    d.setObject(send);
    ws.sendTextMessage(d.toJson());
}

void WebInterface::connected()
{
    connect(&ws,SIGNAL(textMessageReceived(QString)),this,SLOT(onTextMessage(QString)));
    isConnectedP = true;
    emit connectedS();
}

void WebInterface::disconnected()
{
    isConnectedP = false;
    emit disconnectedS();
}

void WebInterface::onTextMessage(QString message)
{
    QJsonDocument d = QJsonDocument::fromJson(message.toUtf8());
    if(!d.isObject())
        return;
    QJsonObject obj = d.object();
    if(!obj.contains("provider"))
        return;
    QString requestType = obj.value("provider").toString("");
    foreach(WebSocketConnector *c, connectors){
        if(c->getRequestType() == requestType){
            c->onMessage(obj.value("data").toObject());
        }
    }
}


