#include "flashscene.h"
#include "websocketserver.h"
#include <algorithm>
#include "channeldevicestate.h"

#define KEY_SMOOTHNESS "smoothness"
#define KEY_DURATION "duration"
#define KEY_TRIGGER "trigger"
#define KEY_COLOR "color"
#define KEY_DEVS "devices"

FlashScene::FlashScene(WebSocketServer* ws, JackProcessor *jackP, VirtualDeviceManager *manager, QString name, QJsonObject serialized) : Scene(name,serialized),
    WebSocketServerProvider(ws),trigger(ws,jackP),filterVDevManager(manager),selectDevManager(&filterVDevManager,ws),
    flashEnabled(false),flashState(),time(),smoothness(0),flashDuration(40),beatSpeed(INT_MAX),timePer(0.0f),colorButton(ws)
{

    ws->registerProvider(this);
    trigger.triggerConfig.insert(Trigger::BEAT);
    connect(&trigger,SIGNAL(trigger()),this,SLOT(triggered()));
    connect(&colorButton,SIGNAL(colorChanged()),this,SLOT(reloadColor()));
    if(serialized.length() > 0){
        if(serialized.contains(KEY_COLOR))
            colorButton.loadSerialized(serialized.value(KEY_COLOR).toObject());
        if(serialized.contains(KEY_TRIGGER))
            trigger.loadSerialized(serialized.value(KEY_TRIGGER).toObject());
        else
            trigger.triggerConfig.insert(Trigger::BEAT);
        if(serialized.contains(KEY_DURATION))
            flashDuration = serialized.value(KEY_DURATION).toDouble(1000);
        if(serialized.contains(KEY_SMOOTHNESS))
            smoothness = serialized.value(KEY_SMOOTHNESS).toDouble(0.2);
    }


    connect(&selectDevManager,SIGNAL(virtualDevicesChanged()),this,SLOT(reloadDevices()));

    filterVDevManager.addAcceptedType(Device::Beamer);
    filterVDevManager.addAcceptedType(Device::RGB);
    filterVDevManager.addAcceptedType(Device::RGBW);

    selectDevManager.deserialize(serialized.value(KEY_DEVS).toObject());

    reloadDevices();
}

void FlashScene::reloadDevices()
{
    QMap<QString,QSharedPointer<Device> > avDevs = selectDevManager.getDevices();
    flashState.clear();
    foreach(QSharedPointer<Device> dev, avDevs.values() ){
        flashState.insert(dev.data()->getDeviceId(),dev.data()->createEmptyState());
    }
    reloadColor();
}

void FlashScene::reloadColor()
{
    QColor color;
    if(colorButton.getColors().length() > 0)
        color = colorButton.getColors().at(0);

    foreach (QSharedPointer<DeviceState> state, flashState) {
        QSharedPointer<ChannelDeviceState> cDevState = state.dynamicCast<ChannelDeviceState>();
        int firstC = cDevState.data()->getFirstChannel();
            cDevState.data()->setChannel(firstC+0,color.redF());
            cDevState.data()->setChannel(firstC+1,color.greenF());
            cDevState.data()->setChannel(firstC+2,color.blueF());
    }
}


QMap<QString, QSharedPointer<DeviceState> > FlashScene::getDeviceState()
{
    float percentage = 0;
    if(flashEnabled){
        if(smoothness > 0.5) smoothness = 0.5;

        int elapsed = time.elapsed();
        if(elapsed < flashDuration*smoothness){//fading in
            percentage = std::min(elapsed / (flashDuration*smoothness),1.0f);
        }
        else if(elapsed < flashDuration - flashDuration*smoothness){ //main flash
            percentage = 1;
        }
        else if(elapsed < flashDuration){ //fading out
            percentage = std::min(1-((elapsed - (flashDuration - flashDuration*smoothness)) /(flashDuration*smoothness)),1.0f);
        }
        else{//readdy
            flashEnabled = false;
        }
    }

    //output
    QMap<QString, QSharedPointer<DeviceState> > ret;
    foreach (QString devId, flashState.keys()) {
        QSharedPointer<DeviceState> state = flashState.value(devId);
        ChannelDeviceState *cDevState = state.dynamicCast<ChannelDeviceState>().data();
        ret.insert(devId,((*cDevState)*percentage).copyToSharedPointer());
    }
    return ret;
}



void FlashScene::start()
{
    trigger.start();
}

void FlashScene::stop()
{
    trigger.stop();
}

void FlashScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject replay;
    replay.insert("trigger",trigger.providerId);
    replay.insert("smoothnessChanged",smoothness);
    replay.insert("durationChanged",flashDuration);
    replay.insert("color",colorButton.providerId);
    replay.insert("devManager",selectDevManager.providerId);
    sendMsg(replay,id,true);
}

void FlashScene::clientUnregistered(QJsonObject msg, int clientIdCounter)
{

}

void FlashScene::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("durationChanged"))
        flashDuration = msg.value("durationChanged").toDouble(100);
    if(msg.contains("smoothnessChanged"))
        smoothness = msg.value("smoothnessChanged").toDouble(0.4);
    sendMsgButNotTo(msg,id,true);
}

QString FlashScene::getRequestType()
{
    return "falshScene";
}

QString FlashScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString FlashScene::getSceneTypeStringStaticaly()
{
    return "flashScene";
}

QJsonObject FlashScene::serialize(){
    QJsonObject ret;
    ret.insert(KEY_TRIGGER,trigger.serialize());
    ret.insert(KEY_SMOOTHNESS,smoothness);
    ret.insert(KEY_DURATION,flashDuration);
    ret.insert(KEY_COLOR,colorButton.serialize());
    ret.insert(KEY_DEVS,selectDevManager.serialize());
    return serializeScene(ret);
}

void FlashScene::triggered()
{
    flashEnabled = true;
    beatSpeed = time.elapsed();
    time.restart();
}
