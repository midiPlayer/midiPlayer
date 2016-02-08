#include "flashscene.h"
#include "websocketserver.h"
#include <algorithm>
#define KEY_SMOOTHNESS "smoothness"
#define KEY_DURATION "duration"
#define KEY_TRIGGER "trigger"
#define KEY_COLOR "color"

FlashScene::FlashScene(WebSocketServer* ws, QList<Device> avDevP, JackProcessor *jackP,QString name,QJsonObject serialized) : Scene(name,serialized),
    WebSocketServerProvider(ws),trigger(ws,jackP),availableDevices(avDevP),
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
    reloadColor();
}

void FlashScene::reloadColor()
{
    flashState.clear();
    QColor color;
    if(colorButton.getColors().length() > 0)
        color = colorButton.getColors().at(0);

    foreach (Device dev, availableDevices) {
        int firstC = dev.getFirstChannel();
        if(dev.getType() == Device::RGB || dev.getType() == Device::RGBW ){
            dev.setChannel(firstC+0,color.redF());
            dev.setChannel(firstC+1,color.greenF());
            dev.setChannel(firstC+2,color.blueF());
        }
        flashState.append(dev);
    }
}


QList<Device> FlashScene::getLights()
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
    QList<Device> ret;
    foreach (Device d, flashState) {
        ret.append(d*percentage);
    }
    return ret;
}

QList<Device> FlashScene::getUsedLights()
{
    if(flashEnabled)
        return flashState;
    else
        return QList<Device>();
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
    return serializeScene(ret);
}

void FlashScene::triggered()
{
    flashEnabled = true;
    beatSpeed = time.elapsed();
    time.restart();
}
