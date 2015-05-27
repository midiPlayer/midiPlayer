/*TODO:
 * - sortieren von Devices nach Raumkoordinaten
 * - zufälliges Device (aber nicht dasselbe) ändern (-> nicht der Reihe nach)
 * - mehrere Änderungen pro Beat
 * - gegenüberliegende Devices gleichzeitig ändern
 * - anbindung an die gui
 * - smoothmodus
*/



#include "beatscene2.h"
#include <QDebug>
#include "websocketserver.h"

#define KEY_SMOOTHNESS "smoothness"
#define KEY_TRIGGER "trigger"
#define MAX_SMOOTHNESS_DUR 2000

BeatScene2::BeatScene2(QList<Device> avDev, JackProcessor* p, WebSocketServer* ws,QString name,QJsonObject serialized) :
    Scene(name,serialized),WebSocketServerProvider(ws),
    c(0,0,0), availableDevices(),trigger(ws,p),
    smoothDuration(0),smoothTimer(),next("next"),prev("prev"), colorButton(ws), nextLamp(0)
{

    QList<QColor> colors;
    colors.append(QColor(254,0,0));
    colors.append(QColor(254,254,0));
    colors.append(QColor(254,0,254));
    colors.append(QColor(0,0,254));
    colors.append(QColor(0,254,0));
    colorButton.setColors(colors);

    foreach (Device dev, avDev) {
        if((dev.getType() == Device::RGB) || (dev.getType() == Device::RGBW))
        availableDevices.append({dev,QColor(0,0,0)});
    }
    if(serialized.length() != 0){
        if(serialized.contains(KEY_SMOOTHNESS)){
            smoothDuration = serialized.value(KEY_SMOOTHNESS).toInt();
        }
    }

    trigger.triggerConfig.insert(Trigger::BEAT);


    next.import(getUsedLights());
    prev.import(getUsedLights());

    connect(&trigger,SIGNAL(trigger()),this,SLOT(changeColor()));
    ws->registerProvider(this);
    smoothTimer.start();
}


QList<Device> BeatScene2::getLights()
{

    if(smoothDuration == 0 || smoothTimer.elapsed() > smoothDuration)
        return next.getLights();
    else{
        float per = float(smoothTimer.elapsed()) / float(smoothDuration);
        if(per>1.0)
            per = 1;
        return prev.passiveFusion(next.getLights(),Device::AV,per);
    }

}

QList<Device> BeatScene2::getUsedLights()
{
    QList<Device> ret;
    foreach (BeatSceneDev dev, availableDevices) {
        ret.append(dev.device);
    }
    return ret;
}

void BeatScene2::stop()
{
    qDebug() << "stoped";
    trigger.stop();
}

void BeatScene2::start()
{
    qDebug() << "started";
    trigger.start();
}

void BeatScene2::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject replay;
    QJsonObject config;
    config.insert("trigger",trigger.providerId);
    config.insert("smoothnessChanged",double(smoothDuration)/double(MAX_SMOOTHNESS_DUR));
    config.insert("colorButton", colorButton.providerId);
    replay.insert("config",config);
    sendMsg(replay,id,true);
}

void BeatScene2::clientUnregistered(QJsonObject msg, int clientIdCounter)
{

}

void BeatScene2::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("smoothnessChanged")){
        smoothDuration = msg.value("smoothnessChanged").toDouble(0)*MAX_SMOOTHNESS_DUR;
    }
    sendMsgButNotTo(msg,id,true);
}

QString BeatScene2::getRequestType()
{
        return "beatScene2";
}

QJsonObject BeatScene2::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_TRIGGER,trigger.serialize());
    ret.insert(KEY_SMOOTHNESS,smoothDuration);
    return serializeScene(ret);
}

QString BeatScene2::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString BeatScene2::getSceneTypeStringStaticaly()
{
    return "beatScene2";
}

void BeatScene2::changeColor()
{   if(availableDevices.length() == 0)
        return;
    QList <QColor> chosenColors=colorButton.getColors();
    QList <QColor> unusedColors = chosenColors;

    foreach (BeatSceneDev dev, availableDevices) {
        unusedColors.removeAll(dev.color);
    }

    if(unusedColors.length() > 0){
        int r = rand() % unusedColors.length(); //Zufallszahl generieren, max. so groß, wie noch freie Farben vorhanden
        BeatSceneDev dev = availableDevices.at(nextLamp);
        dev.color = unusedColors.at(r);
        availableDevices.replace(nextLamp,dev);
        generateNextScene();
    }
    else{
        return;
    }

    nextLamp++;
    if(nextLamp >= availableDevices.length())
        nextLamp = 0;
}

void BeatScene2::generateNextScene() //schreibt die Farbwerte auf die Kanäle
{
    prev.import(getLights());
    QList<Device> ret;
    foreach (BeatSceneDev d, availableDevices) {
        int firstChannel = d.device.getFirstChannel();
        d.device.setChannel(firstChannel + 0,d.color.redF());
        d.device.setChannel(firstChannel + 1,d.color.greenF());
        d.device.setChannel(firstChannel + 2,d.color.blueF());
        ret.append(d.device);
    }
    next.reset();
    next.import(ret);

    smoothTimer.restart();
}
