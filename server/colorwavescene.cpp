#include "colorwavescene.h"
#include "scene.h"
#include "websocketserver.h"
#include "math.h"
#include <QColor>
#include "channeldevicestate.h"
#include "channeldevice.h"

#define KEY_COLOR "color"
#define KEY_TRIGGER "trigger"
#define KEY_SPEED "speed"
#define KEY_ACTIVE_RADIUS "activeRadius"
#define KEY_DEVS "devices"

ColorWaveScene::ColorWaveScene(VirtualDeviceManager *manager, WebSocketServer *ws, JackProcessor *jackP, QString name, QJsonObject serialized):Scene(name,serialized),
    WebSocketServerProvider(ws),filterDeviceManager(manager),selectDevManager(&filterDeviceManager,ws),trigger(ws,jackP),onState(),beatStopwatch(),
    isRunning(false),activeRadius(10),speed(50),colorButton(ws)
{

    connect(&selectDevManager,SIGNAL(virtualDevicesChanged()),this,SLOT(reinitDevices()));

    filterDeviceManager.addAcceptedType(Device::RGB);
    filterDeviceManager.addAcceptedType(Device::RGBW);
    filterDeviceManager.addAcceptedType(Device::Beamer);

    selectDevManager.deserialize(serialized.value(KEY_DEVS).toObject());

    connect(&colorButton,SIGNAL(colorChanged()),this,SLOT(reinitColors()));
    if(serialized.length() > 0){
        if(serialized.contains(KEY_TRIGGER))
            trigger.loadSerialized(serialized.value(KEY_TRIGGER).toObject());
        if(serialized.contains(KEY_COLOR))
            colorButton.loadSerialized(serialized.value(KEY_COLOR).toObject());
        if(serialized.contains(KEY_SPEED))
            speed = serialized.value(KEY_SPEED).toDouble(10);
        if(serialized.contains(KEY_ACTIVE_RADIUS))
            activeRadius = serialized.value(KEY_ACTIVE_RADIUS).toDouble(10);
    }
    else{
     trigger.triggerConfig.insert(Trigger::ONSET);
    }
    connect(&trigger,SIGNAL(trigger()),this,SLOT(triggered()));
    ws->registerProvider(this);
    reinitColors();
}

QMap<QString, QSharedPointer<DeviceState> > ColorWaveScene::getDeviceState()
{
    float radius = beatStopwatch.elapsed()/1000.0 * speed;
    if(!isRunning)
        return QMap<QString, QSharedPointer<DeviceState> >();//empty

    QMap<QString, QSharedPointer<DeviceState> > ret;
    float maxDistance = 0.0;
    foreach(QString devId,onState.keys()){
        QSharedPointer<ChannelDeviceState> state = onState.value(devId).dynamicCast<ChannelDeviceState>();
        float distance = ((center-(selectDevManager.getDevices().value(devId).data()->getPosition())).length());
        maxDistance = std::max(distance,maxDistance);
        float percentage = getPercentageForDistance(distance-radius);
        ret.insert(devId,((*state) * percentage).copyToSharedPointer());
    }
    if(radius > maxDistance + 2*activeRadius)//not interessting anymore
        isRunning = false;
    return ret;
}

void ColorWaveScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject config;
    config.insert("speedChanged",speed);
    config.insert("activeRadiusChanged",activeRadius);
    config.insert("colorButton",colorButton.providerId);
    config.insert("trigger",trigger.providerId);
    config.insert("devManager",selectDevManager.providerId);
    sendMsg(config,id,true);
}

void ColorWaveScene::clientUnregistered(QJsonObject msg, int id)
{

}

void ColorWaveScene::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("speedChanged"))
        speed = msg.value("speedChanged").toDouble(5);
    if(msg.contains("activeRadiusChanged"))
        activeRadius = msg.value("activeRadiusChanged").toDouble(1);
    sendMsgButNotTo(msg,id,true);
}

QString ColorWaveScene::getRequestType()
{
    return "colorWaveScene";
}

void ColorWaveScene::stop()
{
    trigger.stop();
}

void ColorWaveScene::start()
{
    trigger.start();
}

QJsonObject ColorWaveScene::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_TRIGGER,trigger.serialize());
    ret.insert(KEY_COLOR,colorButton.serialize());
    ret.insert(KEY_SPEED,speed);
    ret.insert(KEY_ACTIVE_RADIUS,activeRadius);
    ret.insert(KEY_DEVS,selectDevManager.serialize());
    return serializeScene(ret);
}

QString ColorWaveScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString ColorWaveScene::getSceneTypeStringStaticaly()
{
    return "colorWaveScene";
}

void ColorWaveScene::triggered()
{
    if(isRunning)
        return;
    reinitColors();
    QMap<QString, QSharedPointer<Device> > usedDevices = selectDevManager.getDevices();
    if(usedDevices.count() == 0)
        return;
    int newCenterPos = -1;
    do
        newCenterPos = rand() % usedDevices.count();
    while(centerDevPos == newCenterPos && usedDevices.count() > 1);
    if(newCenterPos == -1)
        return;
    centerDevPos = newCenterPos;
    center = usedDevices.value(usedDevices.keys().at(centerDevPos)).data()->getPosition();
    isRunning = true;
    beatStopwatch.restart();
}

void ColorWaveScene::reinitDevices()
{
    onState.clear();
    QMap<QString, QSharedPointer<Device> > avdev = selectDevManager.getDevices();
    foreach(QString devId, avdev.keys()){
        QSharedPointer<Device> dev = avdev.value(devId);
        onState.insert(devId,dev.data()->createEmptyState());
    }
    reinitColors();
}

void ColorWaveScene::reinitColors()
{

    QColor color;
    if(colorButton.getColors().length() > 0){
        int rIndex = rand() % colorButton.getColors().length();
        color = colorButton.getColors().at(rIndex);
    }

    foreach (QString devId, onState.keys()) {
        QSharedPointer<ChannelDeviceState> cDevState = onState.value(devId).dynamicCast<ChannelDeviceState>();
        int basC = cDevState.data()->getFirstChannel();
        cDevState.data()->setChannel(basC+0,color.redF());
        cDevState.data()->setChannel(basC+1,color.greenF());
        cDevState.data()->setChannel(basC+2,color.blueF());
        if(selectDevManager.getDevices().value(devId).data()->getType() == Device::RGBW){
            cDevState.data()->setChannel(basC+3,0.0);
        }
    }
}
/**
0 * @brief ColorWaveScene::getPercentageForDistance
 * @param distance from the wave trigger bool to the light in meters
 * @return percentage for the light
 */
float ColorWaveScene::getPercentageForDistance(float distance)
{
    if(distance<0 && distance >(-2*activeRadius)){
        float p = distance / (-2*activeRadius);
        p = p*p;
      /*  if(!distance< -activeDistance )//einblenden
            p=p*p*p;*/
      return sin(2*M_PI*p-(M_PI/2))/2+0.5;
    }
    return 0;
}


