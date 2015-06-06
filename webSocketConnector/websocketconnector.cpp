#include "websocketconnector.h"
#include <QDebug>
WebSocketConnector::WebSocketConnector(QObject* parent) :QObject(parent),webInterface(WebInterface::getInstance()),
    requestType(""),urlV(""),requestId(-1),requestParams(),passiveConnected(false)
{
    connect(&webInterface,SIGNAL(connectedS()),this,SLOT(connected()));
    connect(&webInterface,SIGNAL(disconnectedS()),this,SLOT(disconnected()));
}

WebSocketConnector::~WebSocketConnector()
{
    webInterface.unregisterConnector(this);
}


void WebSocketConnector::startConnection(const QString &url)
{
    if(url == "")
        webInterface.closeConnection();
    else{
        qDebug() << "startConnectionCalled";
        webInterface.connectToServer(url);
        urlV = url;
    }
}

QString WebSocketConnector::url() const
{
    return urlV;
}

QString WebSocketConnector::empty() const
{
    return "";
}

QString WebSocketConnector::getRegistrationParamsAsJson()
{
    QJsonDocument d;
    d.setObject(requestParams);
    return d.toJson();
}

QJsonObject WebSocketConnector::getRegistrationParams()
{
    return requestParams;
}

void WebSocketConnector::setRegistrationParams(QString message)
{
    requestParams = QJsonDocument::fromJson(message.toUtf8()).object();
}

QString WebSocketConnector::getRequestType()
{
    return requestType;
}

int WebSocketConnector::getReqestId()
{
    return requestId;
}

void WebSocketConnector::setRequestId(int rqID)
{
    requestId = rqID;
    webInterface.unregisterConnector(this);
    webInterface.registerConnector(this);
}

void WebSocketConnector::registerForRequestType(QString type)
{
    requestType = type;
    if(requestParams.length() == 0)
        webInterface.registerConnector(this);
    else
        webInterface.registerConnector(this,requestParams);
}

void WebSocketConnector::registerForRequestTypePassivly(QString type)
{
     webInterface.registerConnectorPassive(this);
     requestType = type;
     passiveConnected = true;
}

void WebSocketConnector::onMessage(QJsonObject msg)
{
    emit message(msg);
}

void WebSocketConnector::sendMessge(QString msg)
{
    QJsonDocument d = QJsonDocument::fromJson(msg.toUtf8());
    if(requestId == -1)
        webInterface.sendMsg(this,d.object());
    else{
        webInterface.sendMsgToRqId(this,d.object());
    }
}

void WebSocketConnector::setReopen(bool reopen)
{
    webInterface.reopen = reopen;
}

bool WebSocketConnector::getReopen()
{
    return webInterface.reopen;
}

bool WebSocketConnector::isPassive()
{
    return passiveConnected;
}



void WebSocketConnector::connected()
{
    emit connectionSucceded();
}

void WebSocketConnector::disconnected()
{
    emit connectionFailed();
}

