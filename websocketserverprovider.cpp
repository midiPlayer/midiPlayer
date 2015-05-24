#include "websocketserverprovider.h"
#include "websocketserver.h"
WebSocketServerProvider::WebSocketServerProvider(WebSocketServer *s) :
    server(s), connectedSockets(),clientIdCounter(0),providerId(-1)
{

}

void WebSocketServerProvider::registerClient(QJsonObject msg, QWebSocket *client)
{
    if(connectedSockets.contains(client))
        return; //allreaddy registered
    connectedSockets.insert(client,clientIdCounter);
    clientRegistered(msg,clientIdCounter);
    clientIdCounter++;
}

void WebSocketServerProvider::unregisterClient(QJsonObject msg, QWebSocket *client)
{
    if(!connectedSockets.contains(client))
        return; //we don't know him
    int  id = connectedSockets.value(client);
    clientUnregistered(msg,id);
    connectedSockets.remove(client);
}

void WebSocketServerProvider::unregisterClient(QWebSocket *client)
{
    unregisterClient(QJsonObject(),client);
}

void WebSocketServerProvider::onMessage(QJsonObject msg, QWebSocket *client)
{
    if(!connectedSockets.contains(client))
        return;
    int id = connectedSockets.value(client);
    clientMessage(msg,id);
}

bool WebSocketServerProvider::sendMsg(QJsonObject msg, int client_id,bool onlyForProvierID)
{
    QWebSocket* ws = connectedSockets.key(client_id);
    server->sendData(msg,ws,this,onlyForProvierID);
}

bool WebSocketServerProvider::sendMsg(QJsonObject msg,bool onlyForProvierID)
{
    foreach(QWebSocket* ws, connectedSockets.keys()){
        server->sendData(msg,ws,this,onlyForProvierID);
    }
}

bool WebSocketServerProvider::sendMsgButNotTo(QJsonObject msg, int client_id,bool onlyForProvierID)
{
    QWebSocket* notTo = connectedSockets.key(client_id);
    foreach(QWebSocket* ws, connectedSockets.keys()){
        if(ws != notTo)
            server->sendData(msg,ws,this,onlyForProvierID);
    }
}


