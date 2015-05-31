#include "colorwavescene.h"
#include "scene.h"
#include "websocketserver.h"
#include "math.h"
#include <QColor>

#define KEY_COLOR "color"
#define KEY_TRIGGER "trigger"
#define KEY_SPEED "speed"
#define KEY_ACTIVE_RADIUS "activeRadius"

ColorWaveScene::ColorWaveScene(QList<Device> avDev, WebSocketServer *ws, JackProcessor *jackP, QString name, QJsonObject serialized):Scene(name,serialized),
    WebSocketServerProvider(ws),trigger(ws,jackP),usedDevices(),onState(),beatStopwatch(),
    isRunning(false),activeRadius(10),speed(50),colorButton(ws)
{
    foreach (Device dev,avDev) {
        if(dev.getType() == Device::RGB || dev.getType() == Device::RGBW){
            usedDevices.append(dev);
                //dev.setChannel(c,1.0f);//all white
                int basC = dev.getFirstChannel();
                QColor color;
                if(colorButton.getColors().length() > 0)
                    colorButton.getColors().at(0);
                dev.setChannel(basC+0,color.redF());
                dev.setChannel(basC+1,color.greenF());
                dev.setChannel(basC+2,color.blueF());
                if(dev.getType() == Device::RGBW){
                    dev.setChannel(basC+3,0.0);
                }
            onState.append(dev);
        }
    }
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
}

QList<Device> ColorWaveScene::getLights()
{
    float radius = beatStopwatch.elapsed()/1000.0 * speed;
    if(isRunning){
        QList<Device> ret;
        float maxDistance = 0.0;
        foreach(Device dev,onState){
            float distance = ((center-(dev.getPosition())).length());
            maxDistance = std::max(distance,maxDistance);
            float percentage = getPercentageForDistance(distance-radius);
            dev = dev * percentage;
            ret.append(dev);
        }
        if(radius > maxDistance + 2*activeRadius)//not interessting anymore
            isRunning = false;
        return ret;
    }
    return usedDevices;
}

QList<Device> ColorWaveScene::getUsedLights()
{
    return usedDevices;
}

void ColorWaveScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject config;
    config.insert("speedChanged",speed);
    config.insert("activeRadiusChanged",activeRadius);
    config.insert("colorButton",colorButton.providerId);
    config.insert("trigger",trigger.providerId);
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
    int newCenterPos = -1;
    do
        newCenterPos = rand() % usedDevices.length();
    while(centerDevPos == newCenterPos && usedDevices.length() > 1);
    if(newCenterPos == -1)
        return;
    centerDevPos = newCenterPos;
    center = usedDevices.at(centerDevPos).getPosition();
    isRunning = true;
    beatStopwatch.restart();
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


