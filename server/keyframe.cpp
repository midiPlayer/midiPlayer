#include "keyframe.h"
#include "websocketserver.h"
#include <QJsonArray>
/**
 * @brief Keyframe::Keyframe
 * A keyframe ist a point that stores the state of a lamp at a time
 */

#define KEY_KEYFRAME_TIME "keyframe_time"
#define KEY_KEYFRAME_STATE "keyframe_state"

Keyframe::Keyframe(double timeP, DeviceState stateP, WebSocketServer *ws) : state(stateP), time(timeP),
    WebSocketServerProvider(ws),wss(ws)
{
    ws->registerProvider(this);
}

Keyframe::Keyframe(QJsonObject serialized, WebSocketServer *ws) : state(serialized.value(KEY_KEYFRAME_STATE).toObject()),
    time(serialized.value(KEY_KEYFRAME_TIME).toDouble()),WebSocketServerProvider(ws),wss(ws)
{
    ws->registerProvider(this);
}


DeviceState Keyframe::fusionWith(QSharedPointer<Keyframe> later,double now)
{
    if(state.deviceId != later.data()->state.deviceId){
        throw("cannot fusion!");
    }

    double per = (now-time) / (later.data()->time - time);
    return state.fusionWith(later.data()->state,DeviceState::AV,per);
}

QJsonObject Keyframe::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_KEYFRAME_TIME,time);
    ret.insert(KEY_KEYFRAME_STATE,state.serialize());
    return ret;
}

void Keyframe::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject ret;
    ret.insert("time",time);
    ret.insert("state",state.getClientJson());
    sendMsg(ret,id,true);
}

void Keyframe::clientUnregistered(QJsonObject msg, int id)
{

}

void Keyframe::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("valueChnaged")){

    }
    if(msg.contains("deleteKeyframe")){
        sendMsgButNotTo(msg,id,true);
        wss->unregisterProvider(this);
        emit deleteRequested(this);
    }

    if(msg.contains("time")){
        time = msg.value("time").toDouble();
        sendMsgButNotTo(msg,id,true);
    }

    if(msg.contains("state")){
        state.setClientJson(msg.value("state").toArray());
        sendMsgButNotTo(msg,id,true);
    }
}

QString Keyframe::getRequestType()
{
    return "keyframe";
}
