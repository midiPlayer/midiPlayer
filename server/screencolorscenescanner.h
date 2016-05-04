#ifndef SCREENCOLORSCENESCANNER_H
#define SCREENCOLORSCENESCANNER_H
#include "websocketserverprovider.h"
#include <QObject>
#include <QList>
#include <QColor>

class ScreenColorSceneScanner : public QObject, public WebSocketServerProvider
{
Q_OBJECT

public:
    ScreenColorSceneScanner(WebSocketServer *ws);
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
signals:
    void recievedColors(QList<QColor> colors);
};

#endif // SCREENCOLORSCENESCANNER_H
