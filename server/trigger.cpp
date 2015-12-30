#include "trigger.h"
#include "websocketserver.h"

#define KEY_SOURCE "source"
#define KEY_NUM_BEATS "numBeats"
Trigger::Trigger(WebSocketServer* ws, JackProcessor* jackP, QJsonObject serialized) : QObject(0),
    WebSocketServerProvider(ws),jack(jackP),beatCount(0),numBeats(0)
{
    ws->registerProvider(this);
    if(serialized.length() != 0){
        loadSerialized(serialized);
    }

}

void Trigger::start()
{
    connect(jack,SIGNAL(beatNotification()),this,SLOT(beat()));
    connect(jack,SIGNAL(onsetNotification()),this,SLOT(onset()));
}

void Trigger::stop()
{
    disconnect(jack,SIGNAL(beatNotification()),this,SLOT(beat()));
    disconnect(jack,SIGNAL(onsetNotification()),this,SLOT(onset()));
}

void Trigger::clientRegistered(QJsonObject msg, int clientId)
{
    sendMsg(getState(),clientId,true);
}

void Trigger::clientUnregistered(QJsonObject msg, int clientId)
{

}

void Trigger::clientMessage(QJsonObject msg, int clientId)
{
    if(msg.contains("setTrigger")){
        QJsonObject fgt = msg.value("setTrigger").toObject();
        setState(fgt);
        sendMsgButNotTo(getState(),clientId,true);
    }
    if(msg.contains(KEY_NUM_BEATS)){
        numBeats = msg.value(KEY_NUM_BEATS).toInt(0);
        sendMsgButNotTo(msg,clientId,true);
    }
    if(msg.contains("manual")){
        emit trigger();
    }
}

QString Trigger::getRequestType()
{
    return "triggerSrc";
}

QJsonObject Trigger::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_SOURCE,getTriggerSourceJson());
    ret.insert(KEY_NUM_BEATS,numBeats);
    return ret;
}

void Trigger::beat()
{
    if(triggerConfig.contains(BEAT))
        triggerInt();
}

void Trigger::onset()
{

    if(triggerConfig.contains(ONSET))
        triggerInt();
}

QJsonObject Trigger::getTriggerSourceJson()
{
    QJsonObject triggerObj;
    triggerObj.insert("beat",triggerConfig.contains(BEAT));
    triggerObj.insert("onset",triggerConfig.contains(ONSET));
    triggerObj.insert("timer",triggerConfig.contains(TIMER));
    return triggerObj;
}

void Trigger::loadSerialized(QJsonObject serialized)
{
    setState(serialized.value(KEY_SOURCE).toObject());
    if(serialized.contains(KEY_NUM_BEATS)){
        numBeats = serialized.value(KEY_NUM_BEATS).toInt(0);
    }
}

QJsonObject Trigger::getState()
{
    QJsonObject state;
    state.insert("setTrigger",getTriggerSourceJson());
    state.insert(KEY_NUM_BEATS,numBeats);
    return state;
}

void Trigger::setState(QJsonObject fgt)
{
    if(fgt.contains("onset")){
        if(fgt.value("onset").toBool())
            triggerConfig.insert(ONSET);
        else
            triggerConfig.remove(ONSET);
    }
    if(fgt.contains("beat")){
        if(fgt.value("beat").toBool())
            triggerConfig.insert(BEAT);
        else
            triggerConfig.remove(BEAT);
    }
    if(fgt.contains("timer")){
        if(fgt.value("timer").toBool())
            triggerConfig.insert(TIMER);
        else
            triggerConfig.remove(TIMER);
    }
}

void Trigger::triggerInt()
{
    if(++beatCount > numBeats){
        emit trigger();
    }
}
