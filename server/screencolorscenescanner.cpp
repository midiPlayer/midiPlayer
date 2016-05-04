#include "screencolorscenescanner.h"
#include "websocketserver.h"
#include <QJsonArray>
ScreenColorSceneScanner::ScreenColorSceneScanner(WebSocketServer* ws): WebSocketServerProvider(ws)
{
    ws->registerProvider(this);
}

void ScreenColorSceneScanner::clientRegistered(QJsonObject msg, int id)
{

}

void ScreenColorSceneScanner::clientUnregistered(QJsonObject msg, int id)
{

}

void ScreenColorSceneScanner::clientMessage(QJsonObject msg, int id)
{
    QList<QColor> colors;
    foreach(QJsonValue colorName, msg.value("colors").toArray()){
        colors.append(QColor(colorName.toString()));
    }
    emit recievedColors(colors);
}

QString ScreenColorSceneScanner::getRequestType()
{
    return "screenScanner";
}
