#include "colorscene.h"
#include <QListIterator>
#include "websocketserver.h"

#define KEY_COLOR "color"
ColorScene::ColorScene(QList<Device> avDev, WebSocketServer *ws, QString name, QJsonObject serialized) :
    Scene(name,serialized),WebSocketServerProvider(ws), devices(avDev),colorButton(ws)
{
    QList<QColor> colors;
    colors.append(QColor(255,255,255));//initial
    colorButton.setColors(colors);
    reloadColor();
    if(serialized.length() != 0){
        if(serialized.contains(KEY_COLOR))
            colorButton.loadSerialized(serialized.value(KEY_COLOR).toObject());
    }
    connect(&colorButton,SIGNAL(colorChanged()),this,SLOT(reloadColor()));
    ws->registerProvider(this);
}

QList<Device> ColorScene::getUsedLights()
{
    return devices;
}

QJsonObject ColorScene::serialize()
{
    //nothing to erialize as this is an really trivial scenen
    QJsonObject ret;
    ret.insert(KEY_COLOR,colorButton.serialize());
    return serializeScene(ret);
}

QString ColorScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString ColorScene::getSceneTypeStringStaticaly()
{
    return "colorScene";
}

void ColorScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject replay;
    QJsonObject config;
    config.insert("colorButton", colorButton.providerId);
    replay.insert("config",config);
    sendMsg(replay,id,true);
}

void ColorScene::clientUnregistered(QJsonObject msg, int clientIdCounter)
{
}

void ColorScene::clientMessage(QJsonObject msg, int clientIdCounter)
{
}

QString ColorScene::getRequestType()
{
    return "colorScene";
}

void ColorScene::reloadColor()
{
    QList<Device> ret;
    QList<Device>::iterator deviceIter = devices.begin();
    while(deviceIter != devices.end()){
        Device d = *deviceIter;
        QColor c = colorButton.getColors().at(0);
        int first = d.getFirstChannel();
        d.setChannel(first+0,c.redF());
        d.setChannel(first+1,c.greenF());
        d.setChannel(first+2,c.blueF());
        ret.append(d);;
        deviceIter++;
    }
    devices = ret;
}

QList<Device> ColorScene::getLights()
{
    return devices;
}

