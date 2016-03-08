#include "trigger.h"
#include "websocketserver.h"

#define KEY_SOURCE "source"
#define KEY_NUM_BEATS "numBeats"
#define KEY_TIMER_INTERVAL "timerInterval"
#define KEY_TIMER_RANDOM "timerRand"

Trigger::Trigger(WebSocketServer* ws, JackProcessor* jackP, QJsonObject serialized) : QObject(0),
    WebSocketServerProvider(ws),jack(jackP),beatCount(0),numBeats(0),interval(1000),randomness(0.0),
    timer(this), running(false)
{
    ws->registerProvider(this);
    if(serialized.length() != 0){
        loadSerialized(serialized);
    }

    interval = serialized.value(KEY_TIMER_INTERVAL).toInt(interval);
    randomness = serialized.value(KEY_TIMER_RANDOM).toDouble(randomness);

    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
}

void Trigger::start()
{
    running = true;
    connect(jack,SIGNAL(beatNotification()),this,SLOT(beat()));
    connect(jack,SIGNAL(onsetNotification()),this,SLOT(onset()));
    if(triggerConfig.contains(TIMER))
        startTimer();
}

void Trigger::stop()
{
    running = false;
    disconnect(jack,SIGNAL(beatNotification()),this,SLOT(beat()));
    disconnect(jack,SIGNAL(onsetNotification()),this,SLOT(onset()));
    stopTimer();
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
    if(msg.contains(KEY_TIMER_INTERVAL)){
        interval = (int)msg.value(KEY_TIMER_INTERVAL).toDouble(interval);
        stopTimer();
        startTimer();
        sendMsgButNotTo(msg,clientId,true);
    }
    if(msg.contains(KEY_TIMER_RANDOM)){
        randomness = msg.value(KEY_TIMER_RANDOM).toDouble(randomness);
        sendMsgButNotTo(msg,clientId,true);
    }
    if(msg.contains("getState")){
        sendMsg(getState(),clientId,true);
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

    ret.insert(KEY_TIMER_INTERVAL,interval);
    ret.insert(KEY_TIMER_RANDOM,randomness);
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

void Trigger::startTimer()
{
    if(!running)
        return;
    int i = interval + randomness*(2*interval * ((float)rand() / (float)RAND_MAX) - interval);
    timer.start(i);
}

void Trigger::stopTimer()
{
    timer.stop();
}

void Trigger::onTimer()
{
    if(triggerConfig.contains(TIMER)){
        triggerInt();
        startTimer();
    }
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
    state.insert(KEY_TIMER_INTERVAL,interval);
    state.insert(KEY_TIMER_RANDOM,randomness);
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
        if(fgt.value("timer").toBool()){
            triggerConfig.insert(TIMER);
            startTimer();
        }
        else{
            triggerConfig.remove(TIMER);
            stopTimer();
        }
    }
}

void Trigger::triggerInt()
{
    if(++beatCount > numBeats){
        emit trigger();
        beatCount = 0;
    }
}
