#include "dia.h"
#include "websocketserver.h"

#define KEY_SCENE "scene"
#define KEY_NAME "name"
#define KEY_DESC "desc"
#define KEY_FADE_IN_DUR "fadeInDur"
#define KEY_MONITOR "monitor"

Dia::Dia(QJsonObject serialized, SceneBuilder *builder, WebSocketServer *wss,
         JackProcessor *jackP,MonitorIO *monitorP) : WebSocketServerProvider(wss),
        monitorControl(wss,jackP,monitorP,&fadeInDuration,serialized.value(KEY_MONITOR).toObject())
{
    scene = builder->build(serialized.value(KEY_SCENE).toObject());
    name = serialized.value(KEY_NAME).toString("");
    desc = serialized.value(KEY_DESC).toString("");
    fadeInDuration = serialized.value(KEY_FADE_IN_DUR).toDouble(1.0);
    wss->registerProvider(this);
}

Dia::Dia(QSharedPointer<Scene> sceneP, QString nameP, QString descP, float fadeInDurationP,
         WebSocketServer *wss,JackProcessor *jackP,MonitorIO *monitorP): WebSocketServerProvider(wss),
    scene(sceneP),name(nameP),desc(descP),fadeInDuration(fadeInDurationP),monitorControl(wss,jackP,monitorP,&fadeInDuration)
{
    wss->registerProvider(this);
}

QJsonObject Dia::serialize(SceneBuilder *builder)
{
    QJsonObject serialized;
    serialized.insert(KEY_SCENE,builder->serializeScene(scene.data()));
    serialized.insert(KEY_NAME,name);
    serialized.insert(KEY_DESC,desc);
    serialized.insert(KEY_FADE_IN_DUR,fadeInDuration);
    serialized.insert(KEY_MONITOR,monitorControl.serialize());
    return serialized;
}

void Dia::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject reply;
    reply.insert("name",name);
    reply.insert("desc",desc);
    reply.insert("monitorReqId",monitorControl.providerId);
    reply.insert("fadeInDur",fadeInDuration);
    WebSocketServerProvider* wsp = dynamic_cast<WebSocketServerProvider*>(scene.data());
    if(wsp != 0){
     reply.insert("requestType",wsp->getRequestType());
     reply.insert("requestId",wsp->providerId);
    }
    sendMsg(reply,id,true);
}

void Dia::clientUnregistered(QJsonObject msg, int id)
{

}

void Dia::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("fadeInDur")){
        fadeInDuration = msg.value("fadeInDur").toDouble(0.0);
        sendMsgButNotTo(msg,id,true);
    }
    if(msg.contains("desc")){
        desc = msg.value("desc").toString();
        sendMsgButNotTo(msg,id,true);
    }
    if(msg.contains("name")){
        name = msg.value("name").toString();
        sendMsgButNotTo(msg,id,true);
    }
}

QString Dia::getRequestType()
{
    return "dia";
}

void Dia::start()
{
    scene.data()->start();
    monitorControl.start();
}

void Dia::stop()
{
    scene.data()->stop();
    monitorControl.stop();
}


