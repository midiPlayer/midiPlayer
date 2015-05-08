#include "webinterface.h"

void WebInterface::init()
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

bool WebInterface::connect(QString url)
{
    ws.open(QUrl(url));
    return true;
}

WebInterface::WebInterface():connectors(),ws()
{
}


