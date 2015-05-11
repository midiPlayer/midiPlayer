#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H


#include <QList>
#include <QWebSocket>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
class WebSocketConnector;
class WebInterface : public QObject
{
Q_OBJECT
public:
    static WebInterface &getInstance();
    void registerWsConnector(WebSocketConnector *wsc);
    bool connectToServer(QString url);
    bool isConnected();
    void registerConnector(WebSocketConnector *connector);
    void registerConnector(WebSocketConnector *connector, QJsonObject params, bool reregister = false);
    void unregisterConnector(WebSocketConnector *connector);
    void sendMsg(WebSocketConnector *connector,QJsonObject msg);
    void sendMsgToRqId(WebSocketConnector *connector, QJsonObject msg);
    bool reopen;
public slots:
    void connected();
    void disconnected();
    void onTextMessage(QString message);
    void timeout();
signals:
    void connectedS();
    void disconnectedS();
private:
    WebInterface(QObject *parent = 0);
    QList<WebSocketConnector *> connectors;
    QWebSocket ws;
    bool isConnectedP;
    QList<QString> connectedRequestTypes;
    bool onceConnected;
    QString lastUrl;
    QTimer timer;
protected:


};

#endif // WEBINTERFACE_H
