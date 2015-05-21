#include "beatscene1.h"
#include <QDebug>
#include "websocketserver.h"

#define l1 12
#define l2 13
#define l3 14


BeatScene1::BeatScene1(QString name, QList<Device> avDev, JackProcessor* p,WebSocketServer *ws) : Scene(name),WebSocketServerProvider(ws),
    c(0,0,0),highlighted(0,0,0) , availableDevices(avDev),options(),usedDevices(),foregroundTrigger(ws,p),backgroundTrigger(ws,p)
{
    options.append(QColor(255,0,0));
    options.append(QColor(0,255,0));
    options.append(QColor(0,0,255));
    options.append(QColor(255,255,0));
    options.append(QColor(255,0,255));
    options.append(QColor(0,255,255));


    foregroundTrigger.triggerConfig.insert(Trigger::ONSET);
    backgroundTrigger.triggerConfig.insert(Trigger::ONSET);

    usedDevices.clear();
    foreach (Device d, availableDevices) {
        if(d.getType() == Device::Beamer){
            usedDevices.append(d);
        }
    }

    connect(&foregroundTrigger,SIGNAL(trigger()),this,SLOT(changeForeground()));
    connect(&backgroundTrigger,SIGNAL(trigger()),this,SLOT(changeBackground()));
    ws->registerProvider(this);
}

QList<Device> BeatScene1::getLights()
{
    QList<Device> ret;
    foreach (Device d, usedDevices) {
        d.setChannel(0,c.red());
        d.setChannel(1,c.green());
        d.setChannel(2,c.blue());

        d.setChannel(3,highlighted.red());
        d.setChannel(4,highlighted.green());
        d.setChannel(5,highlighted.blue());
        ret.append(d);
    }
    return ret;
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
    replay.insert("config",config);
    sendMsg(replay,id,true);
}

void BeatScene1::clientUnregistered(QJsonObject msg, int id)
{

}

void BeatScene1::clientMessage(QJsonObject msg, int id)
{

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


void BeatScene1::changeForeground()
{
    QColor last = highlighted;
    while(last == highlighted|| c == highlighted)
    {
        int i = double(options.length())*double(rand())/RAND_MAX - 0.0001;
        highlighted = options.at(i);
    }
    qDebug() << c;
}

void BeatScene1::changeBackground()
{
    QColor last = c;
    while(last == c || c == highlighted)
    {
        int i = double(options.length())*double(rand())/RAND_MAX - 0.0001;
        c = options.at(i);
    }
    qDebug() << c;
}



