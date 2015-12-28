#include "keyframescene.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include "websocketserver.h"

#define KEY_KEYFRAMESCENE_KEYFRAMES "key_keyframescene_keyframes"

KeyFrameScene::KeyFrameScene(QList<Device> avDev, QString name, WebSocketServer *ws, QJsonObject serialized) :
    Scene(name,serialized), WebSocketServerProvider(ws),wss(ws)
{
    ws->registerProvider(this);

    foreach (Device dev, avDev) {
        if(dev.getType() == Device::RGBW){//at the moment only rgbw is supported.
            myDevs.append(dev);
        }
    }

    if(serialized.length() != 0 && serialized.contains(KEY_KEYFRAMESCENE_KEYFRAMES)){
        foreach (QJsonValue value, serialized.value(KEY_KEYFRAMESCENE_KEYFRAMES).toArray()) {
            QSharedPointer<Keyframe> pointer = QSharedPointer<Keyframe>(new Keyframe(value.toObject(),wss));
            connect(pointer.data(),SIGNAL(deleteRequested(Keyframe*)),this,SLOT(removeKeyframe(Keyframe*)));
            keyframes.append(pointer);
        }
    }
}

QList<Device> KeyFrameScene::getLights()
{
    QList<Device> ret;
    foreach (Device dev,myDevs) {
        QSharedPointer<Keyframe> min;
        QSharedPointer<Keyframe> max;
        int elapsed = time.elapsed();
        foreach (QSharedPointer<Keyframe> frame, keyframes) {
            if(dev.getDeviceId() == frame.data()->state.deviceId){
                if(frame.data()->time < elapsed && (min.isNull() || frame.data()->time > min.data()->time)){
                    min = frame;
                }
                if(frame.data()->time > elapsed && (max.isNull() || frame.data()->time < max.data()->time)){
                    max = frame;
                }
            }
        }
        if(min.isNull() && max.isNull()){
            dev.setState(min.data()->fusionWith(max));
        }
        else if(min.isNull()){
            dev.setState(min.data()->state);
        }
        else if(max.isNull()){
            dev.setState(max.data()->state);
        }
        ret.append(dev);

    }

    return ret;
}

QList<Device> KeyFrameScene::getUsedLights()
{
    return myDevs;
}

void KeyFrameScene::start()
{
    time.restart();
}

void KeyFrameScene::stop()
{
}

QJsonObject KeyFrameScene::serialize()
{
    QJsonObject ret;
    QJsonArray keyframesJson;
    foreach (QSharedPointer<Keyframe> k, keyframes) {
        keyframesJson.append(k.data()->serialize());
    }
    ret.insert(KEY_KEYFRAMESCENE_KEYFRAMES,keyframesJson);
    return serializeScene(ret);
}



void KeyFrameScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject ret;
    ret.insert("devices",getLampsJson());
    sendMsg(ret,id,true);
}

void KeyFrameScene::clientUnregistered(QJsonObject msg, int id)
{

}

void KeyFrameScene::clientMessage(QJsonObject msg, int id)
{

    if(msg.contains("get_keyframes")){
        QJsonObject ret;
        QJsonArray keyframesJson;
        QString devId = msg.value("get_keyframes").toString();
        foreach (QSharedPointer<Keyframe> k, keyframes) {
            if(k.data()->state.deviceId == devId){
                keyframesJson.append(k.data()->providerId);
            }
        }
        ret.insert("keyframes",keyframesJson);
        sendMsg(ret,id,true);
    }
}

QString KeyFrameScene::getRequestType()
{
    return "keyFrameScene";
}

QString KeyFrameScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString KeyFrameScene::getSceneTypeStringStaticaly()
{
    return "keyFrameScene";
}

void KeyFrameScene::removeKeyframe(Keyframe *which)
{
    for(int i = 0; i < keyframes.length();i++){
        if(keyframes.at(i).data() == which){
            keyframes.removeAt(i);
            break;
        }
    }
}

QJsonArray KeyFrameScene::getLampsJson()
{
    QJsonArray ret;
    foreach (Device dev, myDevs) {
        ret.append(getLampJson(dev));
    }
    return ret;
}

QJsonObject KeyFrameScene::getLampJson(Device dev)
{
    QJsonObject ret;
    ret.insert("devID",dev.getDeviceId());
    return ret;
}
