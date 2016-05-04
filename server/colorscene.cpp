#include "colorscene.h"
#include <QListIterator>
#include "websocketserver.h"
#include "channeldevicestate.h"

#define KEY_COLOR "color"
#define KEY_DEVS "devices"

ColorScene::ColorScene(VirtualDeviceManager *vDevManager, WebSocketServer *ws, QString name, QJsonObject serialized) :
    Scene(name,serialized),WebSocketServerProvider(ws), filterVdevManager(vDevManager),selectDevManager(&filterVdevManager,ws),colorButton(ws), deviceStates()
{

    QList<QColor> colors;
    colors.append(QColor(255,255,255));//initial
    colorButton.setColors(colors);

    connect(&selectDevManager,SIGNAL(virtualDevicesChanged()),this,SLOT(reloadDevices()));

    filterVdevManager.addAcceptedType(Device::Beamer);
    filterVdevManager.addAcceptedType(Device::RGB);
    filterVdevManager.addAcceptedType(Device::RGBW);
    filterVdevManager.addAcceptedType(Device::White);

    selectDevManager.deserialize(serialized.value(KEY_DEVS).toObject());

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
    ret.insert(KEY_DEVS,selectDevManager.serialize());
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
    config.insert("devManager",selectDevManager.providerId);
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
    QMap<QString,QSharedPointer<Device> > avDevs = selectDevManager.getDevices();
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
        cDevState.data()->setRGB(c);
    }

}

