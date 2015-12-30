#include "websocketserver.h"
#define PORT 8888
#include <QJsonArray>

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Light Server"),
  QWebSocketServer::NonSecureMode, this)), m_clients(),providerIdCounter(0)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, PORT)) {
            qDebug() << "WebSocketServer listening";
        connect(m_pWebSocketServer, SIGNAL(newConnection()),this, SLOT(onNewConnection()));
        connect(m_pWebSocketServer, SIGNAL(closed()), this, SLOT(onConnectionClosed()));
    }
}

void WebSocketServer::registerProvider(WebSocketServerProvider *me)
{
    if(!provider.contains(me)){
        provider.append(me);
        me->providerId = providerIdCounter++;
    }
}

void WebSocketServer::unregisterProvider(WebSocketServerProvider *me)
{
    provider.removeAll(me);
}

void WebSocketServer::sendData(QJsonObject data, QWebSocket* reciever, WebSocketServerProvider *provider, bool onlyToProviderId)
{
    QJsonObject msg;
    if(onlyToProviderId)
       msg.insert("providerId",provider->providerId);
    else
        msg.insert("provider",provider->getRequestType());
    msg.insert("data",data);
    QJsonDocument d;
    d.setObject(msg);
    QString msgJson = d.toJson();
    reciever->sendTextMessage(msgJson);
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessage(QString)));
    //connect(pSocket, &QWebSocket::binaryMessageReceived, this, &EchoServer::processBinaryMessage);
    connect(pSocket, SIGNAL(disconnected()), this, SLOT(onConnectionClosed()));

    m_clients << pSocket;
}

void WebSocketServer::onTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject data = jsonResponse.object();
    qDebug() << data;

    if(data.contains("register")){
        QJsonValue value = data.value("register");
        if(value.isArray()){
            foreach(QJsonValue val, value.toArray()){
                foreach (WebSocketServerProvider *p, provider) {
                    if(p->getRequestType() == val.toString("")){
                        p->registerClient(data.value("parameters").toObject(),pClient);
                        break;
                    }
                }
            }
        }
        else if(value.isString()){
            foreach (WebSocketServerProvider *p, provider) {
                if(p->getRequestType() == value.toString("")){
                    p->registerClient(data.value("parameters").toObject(),pClient);
                    break;
                }
            }
        }

    }
    else if(data.contains("registerId")){
        QJsonValue value = data.value("registerId");
        if(value.isArray()){
            foreach(QJsonValue val, value.toArray()){
                foreach (WebSocketServerProvider *p, provider) {
                    if(p->providerId == val.toInt(-2)){
                        p->registerClient(data.value("parameters").toObject(),pClient);
                        break;
                    }
                }
            }
        }
        else{
            foreach (WebSocketServerProvider *p, provider) {
                if(p->providerId == value.toInt(-2)){
                    p->registerClient(data.value("parameters").toObject(),pClient);
                    break;
                }
            }
        }
    }
    else if(data.contains("unregister")){
        QString requestType = data.value("unregister").toString();
        foreach (WebSocketServerProvider *p, provider) {
            if(p->getRequestType() == requestType){
                p->unregisterClient(pClient);
            }
        }
    }
    else if(data.contains("unregisterId")){
        int requestId = data.value("unregisterId").toInt(-2);
        foreach (WebSocketServerProvider *p, provider) {
            if(p->providerId == requestId){
                p->unregisterClient(pClient);
            }
        }
    }
    else if(data.contains("provider")){
        QString rqt = data.value("provider").toString("");
        foreach (WebSocketServerProvider *p, provider) {
            if(!provider.contains(p))
                continue;//provider was deleted
            if(p->getRequestType() == rqt){
                p->onMessage(data.value("data").toObject(),pClient);
            }
        }
    }
    else if(data.contains("providerId")){
        int  pid = data.value("providerId").toInt(-2);
        foreach (WebSocketServerProvider *p, provider) {
            if(!provider.contains(p))
                continue;//provider was deleted
            if(p->providerId == pid){
                p->onMessage(data.value("data").toObject(),pClient);
            }
        }
    }
}

void WebSocketServer::onConnectionClosed()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    foreach (WebSocketServerProvider *p, provider) {
        p->unregisterClient(pClient);
    }
    emit clientClosed();
}


