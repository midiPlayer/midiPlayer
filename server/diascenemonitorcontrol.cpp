#include "diascenemonitorcontrol.h"
#include <QJsonObject>
#include <QJsonArray>
#include "websocketserver.h"

#define KEY_TRIGGER "key_trigger"
#define KEY_RESSOURCES "key_ressources"
#define KEY_BEAT_RANDOM "key_random"
#define KEY_BEAT_FADE_TIME "key_beatFadeTime"

void DiaSceneMonitorControl::loadSerialized(QJsonObject serialized)
{
    if(serialized.contains(KEY_TRIGGER))
        trigger.loadSerialized(serialized.value(KEY_TRIGGER).toObject());


    if(serialized.contains(KEY_RESSOURCES)){
        resources.clear();
        foreach(QJsonValue resJ, serialized.value(KEY_RESSOURCES).toArray()){
            resources.append(resJ.toString());
        }
    }

    if(serialized.contains(KEY_BEAT_RANDOM))
        random = serialized.value(KEY_BEAT_RANDOM).toBool(random);
    if(serialized.contains(KEY_BEAT_FADE_TIME))
        beatFadeTime = serialized.value(KEY_BEAT_FADE_TIME).toDouble(beatFadeTime);
}

DiaSceneMonitorControl::DiaSceneMonitorControl(WebSocketServer *ws,
         JackProcessor *jackP, MonitorIO *monitorP, float *fadeInDurP, QJsonObject serialized): WebSocketServerProvider(ws),
    trigger(ws,jackP),random(true),monitor(monitorP), currentRes(-1),fadeInDur(fadeInDurP)
{
    connect(&trigger,SIGNAL(trigger()),this,SLOT(triggered()));
    ws->registerProvider(this);
    loadSerialized(serialized);

}

void DiaSceneMonitorControl::start()
{
    if(resources.length() < 1){
        monitor->showRessource("",*fadeInDur);
        return;
    }
    trigger.start();
    monitor->showRessource(resources.at(0),*fadeInDur);
    currentRes = 0;
}

void DiaSceneMonitorControl::stop()
{
    trigger.stop();
}

void DiaSceneMonitorControl::clientRegistered(QJsonObject msg, int clientId)
{
    sendMsg(serialize(false),clientId,true);
}

void DiaSceneMonitorControl::clientUnregistered(QJsonObject msg, int clientId)
{
}

void DiaSceneMonitorControl::clientMessage(QJsonObject msg, int clientId)
{
    loadSerialized(msg);
    sendMsgButNotTo(msg,clientId,true);
}

QString DiaSceneMonitorControl::getRequestType()
{
    return "monitorScene";
}

void DiaSceneMonitorControl::triggered()
{
    if(resources.length() > 1){
        int newRes = currentRes;
        if(random){
            while(newRes == currentRes){
                newRes = rand() % resources.length();
            }
        }
        else{
            newRes++;
            if(newRes >= resources.length())
                newRes = 0;
        }
        monitor->showRessource(resources.at(newRes),beatFadeTime);
        currentRes = newRes;
    }
}

QJsonObject DiaSceneMonitorControl::serialize(bool includeTrigger)
{
   QJsonObject ret;
   if(includeTrigger)
    ret.insert(KEY_TRIGGER,trigger.serialize());
   else
    ret.insert(KEY_TRIGGER,trigger.providerId);

   QJsonArray resJson;
   foreach(QString res, resources){
       resJson.append(res);
   }
   ret.insert(KEY_RESSOURCES,resJson);
   ret.insert(KEY_BEAT_RANDOM,random);
   ret.insert(KEY_BEAT_FADE_TIME,beatFadeTime);

   return ret;
}
