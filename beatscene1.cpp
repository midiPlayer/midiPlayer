#include "beatscene1.h"
#include <QDebug>
#include "websocketserver.h"

#define l1 12
#define l2 13
#define l3 14

#define MAX_SMOOTHNESS_DUR 2000


BeatScene1::BeatScene1(QString name, QList<Device> avDev, JackProcessor *p, WebSocketServer *ws) :
    Scene(name),WebSocketServerProvider(ws),
    c(0,0,0),highlighted(0,0,0) , availableDevices(avDev),options(),usedDevices(),foregroundTrigger(ws,p),
    backgroundTrigger(ws,p),smoothDuration(200),smoothTimer(),next("next"),prev("prev")
{
    options.append(QColor(255,0,0));
    options.append(QColor(0,255,0));
    options.append(QColor(0,0,255));
    options.append(QColor(255,255,0));
    options.append(QColor(255,0,255));
    options.append(QColor(0,255,255));

    foregroundTrigger.triggerConfig.insert(Trigger::ONSET);
    backgroundTrigger.triggerConfig.insert(Trigger::BEAT );

    usedDevices.clear();
    foreach (Device d, availableDevices) {
        usedDevices.append(d);
    }

    next.import(usedDevices);
    prev.import(usedDevices);

    connect(&foregroundTrigger,SIGNAL(trigger()),this,SLOT(changeForeground()));
    connect(&backgroundTrigger,SIGNAL(trigger()),this,SLOT(changeBackground()));
    ws->registerProvider(this);
    smoothTimer.start();
}

QList<Device> BeatScene1::getLights()
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

QList<Device> BeatScene1::getUsedLights()
{
    return usedDevices;
}


void BeatScene1::stop()
{
    qDebug() << "stoped";
    foregroundTrigger.stop();
    backgroundTrigger.stop();
}

void BeatScene1::start()
{
    qDebug() << "start";
    foregroundTrigger.start();
    backgroundTrigger.start();
}

void BeatScene1::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject replay;
    QJsonObject config;
    config.insert("foregroundTrigger",foregroundTrigger.providerId);
    config.insert("backgroundTrigger",backgroundTrigger.providerId);
    config.insert("smoothnessChanged",double(smoothDuration)/double(MAX_SMOOTHNESS_DUR));
    replay.insert("config",config);
    sendMsg(replay,id,true);
}

void BeatScene1::clientUnregistered(QJsonObject msg, int id)
{

}

void BeatScene1::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("smoothnessChanged")){
        smoothDuration = msg.value("smoothnessChanged").toDouble(0)*MAX_SMOOTHNESS_DUR;
    }
    sendMsgButNotTo(msg,id,true);
}

QString BeatScene1::getRequestType()
{
    return "beatScene1";
}

QJsonObject BeatScene1::serialize()
{
    QJsonObject ret;
    ret.insert("foregroundTrigger",foregroundTrigger.serialize());
    ret.insert("backgroundTrigger",backgroundTrigger.serialize());
    return ret;
}

QString BeatScene1::getSceneTypeString()
{
    return "beatScene1";
}


void BeatScene1::changeForeground()
{
    QColor last = highlighted;
    while(last == highlighted|| c == highlighted)
    {
        int i = double(options.length())*double(rand())/RAND_MAX - 0.0001;
        highlighted = options.at(i);
    }
    generateNextScene();
}

void BeatScene1::changeBackground()
{
    QColor last = c;
    while(last == c || c == highlighted)
    {
        int i = double(options.length())*double(rand())/RAND_MAX - 0.0001;
        c = options.at(i);
    }
    generateNextScene();
}

void BeatScene1::generateNextScene()
{
    prev.import(getLights());
    QList<Device> ret;
    foreach (Device d, usedDevices) {
        int firstChannel = d.getFirstChannel();
        d.setChannel(firstChannel + 0,c.red()/255.0f);
        d.setChannel(firstChannel + 1,c.green()/255.0f);
        d.setChannel(firstChannel + 2,c.blue()/255.0f);

        if(d.getType() == Device::Beamer){
            d.setChannel(firstChannel + 3,highlighted.red()/255.0f);
            d.setChannel(firstChannel + 4,highlighted.green()/255.0f);
            d.setChannel(firstChannel + 5,highlighted.blue()/255.0f);
        }
        ret.append(d);
    }
    next.reset();
    next.import(ret);

    smoothTimer.restart();
}



