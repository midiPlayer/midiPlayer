#include "keyframescene.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>

#define KEY_KEYFRAMESCENE_KEYFRAMES "key_keyframescene_keyframes"

KeyFrameScene::KeyFrameScene(QList<Device> avDev, QString name, QJsonObject serialized) : Scene(name,serialized)
{
    foreach (Device dev, avDev) {
        if(dev.getType() == Device::RGBW){//at the moment only rgbw is supported.
            myDevs.append(dev);
        }
    }

    if(serialized.length() != 0 && serialized.contains(KEY_KEYFRAMESCENE_KEYFRAMES)){
        foreach (QJsonValue value, serialized.value(KEY_KEYFRAMESCENE_KEYFRAMES).toArray()) {
            keyframes.append(Keyframe(value.toObject()));
        }
    }
}

QList<Device> KeyFrameScene::getLights()
{
    QList<Device> ret;
    foreach (Device dev,myDevs) {
        Keyframe *min = 0;
        Keyframe *max = 0;
        int elapsed = time.elapsed();
        foreach (Keyframe frame, keyframes) {
            if(dev.getDeviceId() == frame.state.deviceId){
                if(frame.time < elapsed && (min == 0 || frame.time > min->time)){
                    min = &frame;
                }
                if(frame.time > elapsed && (max == 0 || frame.time < max->time)){
                    max = &frame;
                }
            }
        }
        if(min != 0 && max != 0){
            dev.setState(min->fusionWith(*max));
        }
        else if(min != 0){
            dev.setState(min->state);
        }
        else if(max != 0){
            dev.setState(max->state);
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
    foreach (Keyframe k, keyframes) {
        keyframesJson.append(k.serialize());
    }
    ret.insert(KEY_KEYFRAMESCENE_KEYFRAMES,keyframesJson);
    return serializeScene(ret);
}
