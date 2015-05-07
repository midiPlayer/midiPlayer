#include "websocketserver.h"
#define PORT 8888
#include <QJsonArray>

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent),m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Light Server"),
                                                                          QWebSocketServer::NonSecureMode, this)), m_clients()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, PORT)) {
            qDebug() << "WebSocketServer listening";
        connect(m_pWebSocketServer, SIGNAL(newConnection()),this, SLOT(onNewConnection()));
        connect(m_pWebSocketServer, SIGNAL(closed()), this, SLOT(onConnectionClosed()));
    }
}

void WebSocketServer::registerProvider(WebSocketServerProvider *me)
{
    if(!provider.contains(me))
        provider.append(me);
}

void WebSocketServer::unregisterProvider(WebSocketServerProvider *me)
{
    provider.removeAll(me);
}

void WebSocketServer::sendData(QJsonObject data, QWebSocket* reciever, WebSocketServerProvider *provider)
{
    QJsonObject msg;
    msg.insert("provider",provider->getRequestType());
    msg.insert("data",data);
    QJsonDocument d;
    d.fromVariant(msg);
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
                qDebug() << val;
                foreach (WebSocketServerProvider *p, provider) {
                    if(p->getRequestType() == val.toString("")){
                        p->registerClient(data.value("parameters").toObject(),pClient);
                        break;
                    }
                }
            }
        }
    }
}

void WebSocketServer::onConnectionClosed()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

}

