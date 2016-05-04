#include "screencolorscene.h"
#include "channeldevicestate.h"
#include "websocketserver.h"

#define KEY_SELECTED_DEVS "selected_devices"

ScreenColorScene::ScreenColorScene(VirtualDeviceManager *manager, WebSocketServer *ws, QString name, QJsonObject serialized) :
    WebSocketServerProvider(ws),
    Scene(name,serialized), scanner(ws), filterDevManager(manager),
    selectDevManager(&filterDevManager,ws,serialized.value(KEY_SELECTED_DEVS).toObject())
{
    ws->registerProvider(this);
    connect(&filterDevManager,SIGNAL(virtualDevicesChanged()),&selectDevManager,SLOT(updateDevices()));
    connect(&selectDevManager,SIGNAL(virtualDevicesChanged()),this,SLOT(updateDevices()));
    filterDevManager.addAcceptedType(Device::RGB);
    filterDevManager.addAcceptedType(Device::RGBW);

    connect(&scanner,SIGNAL(recievedColors(QList<QColor>)),this,SLOT(setColors(QList<QColor>)));
}

QJsonObject ScreenColorScene::serialize()
{
    return serializeScene();
}

QMap<QString, QSharedPointer<DeviceState> > ScreenColorScene::getDeviceState()
{
    return state;
}

QString ScreenColorScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString ScreenColorScene::getSceneTypeStringStaticaly()
{
    return "screenColor";
}

void ScreenColorScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject config;
    config.insert("devManager",selectDevManager.providerId);
    sendMsg(config,id,true);
}

void ScreenColorScene::clientUnregistered(QJsonObject msg, int clientIdCounter)
{

}

void ScreenColorScene::clientMessage(QJsonObject msg, int clientIdCounter)
{

}

QString ScreenColorScene::getRequestType()
{
    return "screenColor";
}


void ScreenColorScene::updateDevices()
{
    state.clear();
    foreach(QSharedPointer<Device> dev, selectDevManager.getDevices().values() ){
        state.insert(dev.data()->getDeviceId(),dev.data()->createEmptyState());
    }
    updateColor();
}

void ScreenColorScene::setColors(QList<QColor> colors)
{
    this->colors = colors;
    updateColor();
}

void ScreenColorScene::updateColor()
{
    if(colors.length() > 1){
        qDebug() << "WARNING: currently only one color is supported!";
        return;
    }
    QColor c(0,0,0);

    if(colors.length() > 0){
        c = colors.first();
    }

    foreach(QSharedPointer<DeviceState> s, state){
        QSharedPointer<ChannelDeviceState> cDevState = s.dynamicCast<ChannelDeviceState>();
        cDevState.data()->setRGB(c);
    }
}
