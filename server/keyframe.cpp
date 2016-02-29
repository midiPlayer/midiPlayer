#include "keyframe.h"
#include "websocketserver.h"
#include <QJsonArray>
#include "channeldevice.h"
/**
 * @brief Keyframe::Keyframe
 * A keyframe ist a point that stores the state of a lamp at a time
 */

#define KEY_KEYFRAME_TIME "keyframe_time"
#define KEY_KEYFRAME_STATE "keyframe_state"

Keyframe::Keyframe(double timeP, ChannelDeviceState stateP, WebSocketServer *ws) : state(stateP), time(timeP),
    WebSocketServerProvider(ws),wss(ws),liveEditing(false),liveEditor(-1)
{
    ws->registerProvider(this);
}

Keyframe::Keyframe(QJsonObject serialized, WebSocketServer *ws,VirtualDeviceManager * manager) : state(serialized.value(KEY_KEYFRAME_STATE).toObject(),manager),
    time(serialized.value(KEY_KEYFRAME_TIME).toDouble()),WebSocketServerProvider(ws),wss(ws),liveEditing(false), liveEditor(-1)
{
    ws->registerProvider(this);
}


ChannelDeviceState Keyframe::fusionWith(QSharedPointer<Keyframe> later,double now)
{
    if(state.getDevice()->getDeviceId() != later.data()->state.getDevice()->getDeviceId()){
        throw("cannot fusion!");
    }

    double per = (now-time) / (later.data()->time - time);
    return ChannelDeviceState(state.fusionWith(QSharedPointer<DeviceState>(later.data()->state.copyToSharedPointer()),ChannelDeviceState::AV,per).dynamicCast<ChannelDeviceState>().data());
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

    if(liveEditor == id){
        liveEditing = false;
    }
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

    if(msg.contains("liveEditing")){
        liveEditing = msg.value("liveEditing").toBool(false);
        liveEditor = id;
    }
}

QString Keyframe::getRequestType()
{
    return "keyframe";
}

bool Keyframe::isLiveEditing()
{
    return liveEditing;
}

void Keyframe::beforeDelete()
{
    QJsonObject del;
    del.insert("deleteKeyframe",true);
    sendMsg(del,true);
}
