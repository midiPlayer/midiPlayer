#include "trigger.h"
#include "websocketserver.h"
Trigger::Trigger(WebSocketServer *ws,JackProcessor *jackP) : QObject(0),WebSocketServerProvider(ws),jack(jackP)
{
    ws->registerProvider(this);
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
        sendMsgButNotTo(getState(),clientId);
    }
}

QString Trigger::getRequestType()
{
    return "triggerSrc";
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

QJsonObject Trigger::getState()
{
    QJsonObject state;
    QJsonObject triggerObj;
    triggerObj.insert("beat",triggerConfig.contains(BEAT));
    triggerObj.insert("onset",triggerConfig.contains(ONSET));
    triggerObj.insert("timer",triggerConfig.contains(TIMER));
    state.insert("setTrigger",triggerObj);
    return state;
}
