#include "colorbutton.h"
#include "websocketserver.h"
#define KEY_COLORS "colors"

ColorButton::ColorButton(WebSocketServer*ws,QJsonObject serialized):QObject(0), WebSocketServerProvider(ws)
{
    ws->registerProvider(this);
    if(serialized.length() != 0)
    loadSerialized(serialized);
}

void ColorButton::clientRegistered(QJsonObject msg, int clientId)
{
    sendMsg(getState(),clientId,true);
}

void ColorButton::clientUnregistered(QJsonObject msg, int clientId)
{


}

void ColorButton::loadSerialized(QJsonObject serialized)
{
    if(serialized.contains(KEY_COLORS)){
        colors.clear();
        loadColosFromString(serialized.value(KEY_COLORS).toString(""));
    }
}

void ColorButton::loadColosFromString(QString colorString)
{
    colors.clear();
    QStringList colorsRead = colorString.split(",", QString::SkipEmptyParts);
    foreach (QString colorString, colorsRead) {
        colors.append(QColor(colorString));
    }
}

void ColorButton::clientMessage(QJsonObject msg, int clientId)
{
    if(msg.contains("colorChanged")){
        QString colorString = msg.value("colorChanged").toString("");
        loadColosFromString(colorString);
        emit colorChanged();
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

QString ColorButton::getColorString()
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

    return colorString;
}

QJsonObject ColorButton::getState()
{
    QString colorString = getColorString();
    QJsonObject message;
    message.insert("colorChanged", colorString);
    return message;
}

QJsonObject ColorButton::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_COLORS,getColorString());
    return ret;
}



