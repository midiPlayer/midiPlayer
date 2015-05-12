#include "webinterface.h"
#include "websocketconnector.h"

WebInterface::WebInterface(QObject *parent):QObject(parent), connectors(),ws(),isConnectedP(false),reopen(false),
    onceConnected(false),timer(),lastUrl("")
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));

    connect(&ws,SIGNAL(connected()),this,SLOT(connected()));
    connect(&ws,SIGNAL(disconnected()),this,SLOT(disconnected()));
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
    lastUrl = url;
    ws.open(QUrl(url));
    return true;
}

bool WebInterface::isConnected()
{
    return isConnectedP;
}

void WebInterface::registerConnector(WebSocketConnector *connector){
    registerConnector(connector,QJsonObject());
}

void WebInterface::registerConnector(WebSocketConnector *connector,QJsonObject params,bool reregister)
{
    if(!reregister)
        connectors.append(connector);
    if(connectedRequestTypes.contains(connector->getRequestType()) && connector->getReqestId() == -1)
        return; //allreaddy requested
    connectedRequestTypes.append(connector->getRequestType());
    QJsonObject msg;
    if(connector->getReqestId() == -1)
        msg.insert("register",connector->getRequestType());
    else
        msg.insert("registerId",connector->getReqestId());
    QJsonDocument d;
    if(params.length() != 0)
    msg.insert("parameters",params);
    d.setObject(msg);
    ws.sendTextMessage(d.toJson());
}

void WebInterface::unregisterConnector(WebSocketConnector *connector)
{
    connectors.removeAll(connector);
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

void WebInterface::sendMsgToRqId(WebSocketConnector *connector, QJsonObject msg)
{
    QJsonObject send;
    send.insert("providerId",connector->getReqestId());
    send.insert("data",msg);
    QJsonDocument d;
    d.setObject(send);
    ws.sendTextMessage(d.toJson());
}

void WebInterface::connected()
{
    qDebug() << "connected";
    connect(&ws,SIGNAL(textMessageReceived(QString)),this,SLOT(onTextMessage(QString)));
    isConnectedP = true;
    onceConnected = true;
    emit connectedS();

    connectedRequestTypes.clear();
    foreach (WebSocketConnector *connect, connectors) {
        registerConnector(connect,connect->getRegistrationParams(),true);
    }
}

void WebInterface::disconnected()
{
    qDebug() << "failed";
    isConnectedP = false;
    emit disconnectedS();
    if(onceConnected && reopen && lastUrl != ""){
        if(!timer.isActive()){
            timer.stop();
            timer.start(10*1000);
            qDebug() << "disconnected next retry in 10s";
        }
    }
}

void WebInterface::onTextMessage(QString message)
{
  //  qDebug() << "got message: " << message;
    QJsonDocument d = QJsonDocument::fromJson(message.toUtf8());
    if(!d.isObject())
        return;
    QJsonObject obj = d.object();
    if(obj.contains("provider")){
        QString requestType = obj.value("provider").toString("");
        foreach(WebSocketConnector *c, connectors){
            if(c->getRequestType() == requestType && c->getReqestId() == -1){
                c->onMessage(obj.value("data").toObject());
            }
        }
    }
    else if(obj.contains("providerId")){
        int requestId = obj.value("providerId").toInt(-2);//so we don't fit objects with id = -1
        foreach(WebSocketConnector *c, connectors){
            if(c->getReqestId() == requestId)
                c->onMessage(obj.value("data").toObject());
        }
    }
}

void WebInterface::timeout()
{
    timer.stop();
    connectToServer(lastUrl);
}


