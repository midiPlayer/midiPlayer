#include "colorscene.h"
#include <QListIterator>
#include "websocketserver.h"
#include "channeldevicestate.h"

#define KEY_COLOR "color"
ColorScene::ColorScene(VirtualDeviceManager *vDevManager, WebSocketServer *ws, QString name, QJsonObject serialized) :
    Scene(name,serialized),WebSocketServerProvider(ws), filterVdevManager(vDevManager),colorButton(ws), deviceStates()
{

    QList<QColor> colors;
    colors.append(QColor(255,255,255));//initial
    colorButton.setColors(colors);

    filterVdevManager.addAcceptedType(Device::Beamer);
    filterVdevManager.addAcceptedType(Device::RGB);
    filterVdevManager.addAcceptedType(Device::RGBW);
    connect(&filterVdevManager,SIGNAL(virtualDevicesChanged()),this,SLOT(reloadDevices()));
    reloadDevices();

    connect(&colorButton,SIGNAL(colorChanged()),this,SLOT(reloadColor()));
    if(serialized.length() != 0){
        if(serialized.contains(KEY_COLOR))
            colorButton.loadSerialized(serialized.value(KEY_COLOR).toObject());
    }
    ws->registerProvider(this);
}

QJsonObject ColorScene::serialize()
{
    //nothing to serialize as this is an really trivial scene
    QJsonObject ret;
    ret.insert(KEY_COLOR,colorButton.serialize());
    return serializeScene(ret);
}

QMap<QString, QSharedPointer<DeviceState> > ColorScene::getDeviceState()
{
    return deviceStates;
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

void ColorScene::reloadDevices()
{
    QMap<QString,QSharedPointer<Device> > avDevs = filterVdevManager.getDevices();
    deviceStates.clear();
    foreach(QSharedPointer<Device> dev, avDevs.values() ){
        deviceStates.insert(dev.data()->getDeviceId(),dev.data()->createEmptyState());
    }
    reloadColor();
}

void ColorScene::reloadColor()
{
    QColor c = colorButton.getColors().at(0);
    foreach(QSharedPointer<DeviceState> state, deviceStates){
        QSharedPointer<ChannelDeviceState> cDevState = state.dynamicCast<ChannelDeviceState>();
        int first = cDevState.data()->getFirstChannel();
        cDevState.data()->setChannel(first+0,c.redF());
        cDevState.data()->setChannel(first+1,c.greenF());
        cDevState.data()->setChannel(first+2,c.blueF());
    }
}

