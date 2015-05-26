#ifndef COLORBUTTON_H
#define COLORBUTTON_H
#include "websocketserverprovider.h"
#include <QColor>
#include <QList>


class ColorButton: public WebSocketServerProvider
{
public:
    ColorButton(WebSocketServer *ws);

    void clientRegistered(QJsonObject msg,int clientId);
    void clientUnregistered(QJsonObject msg,int clientId);
    void clientMessage(QJsonObject msg,int clientId);
    QString getRequestType();

    QList<QColor> getColors() const;
    void setColors(const QList<QColor> &value);

private:
    QList<QColor>colors;
    QJsonObject sendState();
};

#endif // COLORBUTTON_H
