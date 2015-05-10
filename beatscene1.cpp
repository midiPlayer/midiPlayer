#include "beatscene1.h"
#include <QDebug>
#include "websocketserver.h"

#define l1 12
#define l2 13
#define l3 14


BeatScene1::BeatScene1(QString name, QList<Device> avDev, JackProcessor* p,WebSocketServer *ws) : Scene(name),WebSocketServerProvider(ws),
    c(0,0,0),highlighted(0,0,0) , availableDevices(avDev),options(),usedDevices(),foregroundTrigger(),backgroundTrigger()
{
    jackProcessor = p;
    options.append(QColor(255,0,0));
    options.append(QColor(0,255,0));
    options.append(QColor(0,0,255));
    options.append(QColor(255,255,0));
    options.append(QColor(255,0,255));
    options.append(QColor(0,255,255));


  //  foregroundTrigger.insert(ONSET);
  //   backgroundTrigger.insert(BEAT);

    usedDevices.clear();
    foreach (Device d, availableDevices) {
        if(d.getType() == Device::Beamer){
            usedDevices.append(d);
        }
    }
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
    disconnect(jackProcessor,SIGNAL(beatNotification()),this,SLOT(beat()));
    disconnect(jackProcessor,SIGNAL(onsetNotification()),this,SLOT(onset()));
}

void BeatScene1::start()
{
    qDebug() << "start";
    connect(jackProcessor,SIGNAL(beatNotification()),this,SLOT(beat()));
    connect(jackProcessor,SIGNAL(onsetNotification()),this,SLOT(onset()));
}

void BeatScene1::clientRegistered(QJsonObject msg, int id)
{
    sendMsg(getState(true,true),id);
}

void BeatScene1::clientUnregistered(QJsonObject msg, int id)
{

}

void BeatScene1::clientMessage(QJsonObject msg, int id)
{
    bool fg = false;
    bool bg = false;
    if(msg.contains("foregroundTrigger")){
        QJsonObject fgt = msg.value("foregroundTrigger").toObject();
        parseTriggerMsg(fgt,&foregroundTrigger);
        fg = true;
    }
    if(msg.contains("backgroundTrigger")){
        QJsonObject fgt = msg.value("backgroundTrigger").toObject();
        parseTriggerMsg(fgt,&backgroundTrigger);
        bg = true;
    }
    sendMsgButNotTo(getState(fg,bg),id);
}

QString BeatScene1::getRequestType()
{
    return "beatScene1";
}

void BeatScene1::beat()
{
    if(foregroundTrigger.contains(BEAT))
        changeForeground();
    if(backgroundTrigger.contains(BEAT))
        changeBackground();
}

void BeatScene1::onset()
{
    if(foregroundTrigger.contains(ONSET))
        changeForeground();
    if(backgroundTrigger.contains(ONSET))
        changeBackground();
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

void BeatScene1::parseTriggerMsg(QJsonObject msg, QSet<BeatScene1::TriggerType> *trigger)
{
    if(msg.contains("onset")){
        if(msg.value("onset").toBool())
            trigger->insert(ONSET);
        else
            trigger->remove(ONSET);
    }
    if(msg.contains("beat")){
        if(msg.value("beat").toBool())
            trigger->insert(BEAT);
        else
            trigger->remove(BEAT);
    }
    if(msg.contains("timer")){
        if(msg.value("timer").toBool())
            trigger->insert(TIMER);
        else
            trigger->remove(TIMER);
    }
}

QJsonObject BeatScene1::getState(bool foreground,bool background)
{
    QJsonObject state;

    if(foreground){
        QJsonObject foregroundTriggerObj;
        foregroundTriggerObj.insert("beat",foregroundTrigger.contains(BEAT));
        foregroundTriggerObj.insert("onset",foregroundTrigger.contains(ONSET));
        foregroundTriggerObj.insert("timer",foregroundTrigger.contains(TIMER));
        state.insert("foregroundTrigger",foregroundTriggerObj);
    }
    if(background)
    {
        QJsonObject backgroundTriggerObj;
        backgroundTriggerObj.insert("beat",backgroundTrigger.contains(BEAT));
        backgroundTriggerObj.insert("onset",backgroundTrigger.contains(ONSET));
        backgroundTriggerObj.insert("timer",backgroundTrigger.contains(TIMER));
        state.insert("backgroundTrigger",backgroundTriggerObj);
    }

    return state;
}



