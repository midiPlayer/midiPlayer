#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H


#include <QList>
#include <QWebSocket>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
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
    void unregisterConnector(WebSocketConnector *connector);
    void sendMsg(WebSocketConnector *connector,QJsonObject msg);
public slots:
    void connected();
    void disconnected();
    void onTextMessage(QString message);
signals:
    void connectedS();
    void disconnectedS();
private:
    WebInterface(QObject *parent = 0);
    QList<WebSocketConnector *> connectors;
    QWebSocket ws;
    bool isConnectedP;
    QList<QString> connectedRequestTypes;
protected:


};

#endif // WEBINTERFACE_H
