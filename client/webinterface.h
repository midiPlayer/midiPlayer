#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include "websocketconnector.h"
#include <QList>
#include <QWebSocket>

class WebInterface
{
public:
    static void init();
    static WebInterface &getInstance();
    void registerWsConnector(WebSocketConnector *wsc);
    bool connect(QString url);
private:
    WebInterface();
    QList<WebSocketConnector *> connectors;
    QWebSocket ws;
protected:


};

#endif // WEBINTERFACE_H
