#include "colorbutton.h"
#include "websocketserver.h"


ColorButton::ColorButton(WebSocketServer*ws):WebSocketServerProvider(ws)
{
    ws->registerProvider(this);
}

void ColorButton::clientRegistered(QJsonObject msg, int clientId)
{
    sendMsg(sendState(),clientId,true);
}

void ColorButton::clientUnregistered(QJsonObject msg, int clientId)
{

}

void ColorButton::clientMessage(QJsonObject msg, int clientId)
{
    if(msg.contains("colorChanged")){
        QString colorString = msg.value("colorChanged").toString("");
        colors.clear();
        QStringList colorsRead = colorString.split(",", QString::SkipEmptyParts);
        foreach (QString colorString, colorsRead) {
            colors.append(QColor(colorString));
        }
    }
    sendMsgButNotTo(msg, clientId, true);
}

QString ColorButton::getRequestType()
{
    return "colorButton";
}

QList<QColor> ColorButton::getColors() const
{
    return colors;
}

void ColorButton::setColors(const QList<QColor> &value)
{
    colors = value;
}

QJsonObject ColorButton::sendState()
{
    QString colorString;
    bool isFirst = true;
    foreach (QColor color, colors) {
        if(!isFirst){
            colorString+=",";
        }
        isFirst = false;
        colorString+=color.name();
    }
    QJsonObject message;
    message.insert("colorChanged", colorString);
    return message;
}


