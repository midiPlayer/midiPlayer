#include "trigger.h"
#include "websocketserver.h"

#define KEY_SOURCE "source"
Trigger::Trigger(WebSocketServer* ws, JackProcessor* jackP, QJsonObject serialized) : QObject(0),
    WebSocketServerProvider(ws),jack(jackP)
{
    ws->registerProvider(this);
    if(serialized.length() != 0){
        setState(serialized.value(KEY_SOURCE).toObject());
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
        sendMsgButNotTo(getState(),clientId);
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
    return Serializable::serialize(ret);
}

void Trigger::beat()
{
    if(triggerConfig.contains(BEAT))
        emit trigger();
}

void Trigger::onset()
{

    if(triggerConfig.contains(ONSET))
        emit trigger();
}

QJsonObject Trigger::getTriggerSourceJson()
{
    QJsonObject triggerObj;
    triggerObj.insert("beat",triggerConfig.contains(BEAT));
    triggerObj.insert("onset",triggerConfig.contains(ONSET));
    triggerObj.insert("timer",triggerConfig.contains(TIMER));
    return triggerObj;
}

QJsonObject Trigger::getState()
{
    QJsonObject state;
    state.insert("setTrigger",getTriggerSourceJson());
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
