#include "keyframescene.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include "websocketserver.h"
#include "whitedevice.h"

#define KEY_MUSIC_PLAYER "music_player"

#define KEY_KEYFRAMESCENE_KEYFRAMES "key_keyframescene_keyframes"

KeyFrameScene::KeyFrameScene(QList<QSharedPointer<Device> > avDev, QString name, WebSocketServer *ws, QJsonObject serialized) :
    Scene(name,serialized), WebSocketServerProvider(ws),wss(ws),watch(ws),
    musicPlayer(ws,serialized.value(KEY_MUSIC_PLAYER).toObject())
{
    ws->registerProvider(this);

    connect(&watch,SIGNAL(resumed()),this,SLOT(handleResume()));
    connect(&watch,SIGNAL(started()),this,SLOT(handleResume()));
    connect(&watch,SIGNAL(stoped()),&musicPlayer,SLOT(stop()));
    connect(&watch,SIGNAL(timeSet()),this,SLOT(handleTimeChanged()));

    foreach (QSharedPointer <Device> dev, avDev) {
        myDevs.append(dev);//use all devs
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
    foreach (QSharedPointer<Device> devPrt,myDevs) {
        Device dev(devPrt.data());
        QSharedPointer<Keyframe> min;
        QSharedPointer<Keyframe> max;
        double elapsed = (double)watch.getMSecs() / 1000.0;
        foreach (QSharedPointer<Keyframe> frame, keyframes) {
            if(dev.getDeviceId() == frame.data()->state.deviceId){
                if(frame.data()->isLiveEditing()){//override
                    min = frame;
                    max = QSharedPointer<Keyframe>();
                    break;
                }
                if(frame.data()->time < elapsed && (min.isNull() || frame.data()->time > min.data()->time)){
                    min = frame;
                }
                if(frame.data()->time > elapsed && (max.isNull() || frame.data()->time < max.data()->time)){
                    max = frame;
                }
            }
        }
        if(!min.isNull() && !max.isNull()){
            dev.setState(min.data()->fusionWith(max,elapsed));
        }
        else if(!min.isNull()){
            dev.setState(min.data()->state);
        }
        else if(!max.isNull()){
            dev.setState(max.data()->state);
        }
        ret.append(dev);

    }

    return ret;
}

QList<Device> KeyFrameScene::getUsedLights()
{
    QList<Device> ret;
    foreach (QSharedPointer<Device> dev, myDevs) {
        ret.append(Device(dev.data()));
    }
    return ret;
}

void KeyFrameScene::start()
{
    watch.start();
    foreach (QSharedPointer<Keyframe> k, keyframes) {
        k.data()->liveEditing = false;
    }
    musicPlayer.play();
}

void KeyFrameScene::stop()
{
    watch.stop();
    musicPlayer.stop();
}

QJsonObject KeyFrameScene::serialize()
{
    QJsonObject ret;
    QJsonArray keyframesJson;
    foreach (QSharedPointer<Keyframe> k, keyframes) {
        keyframesJson.append(k.data()->serialize());
    }
    ret.insert(KEY_KEYFRAMESCENE_KEYFRAMES,keyframesJson);
    ret.insert(KEY_MUSIC_PLAYER,musicPlayer.serialize());
    return serializeScene(ret);
}



void KeyFrameScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject ret;
    ret.insert("devices",getLampsJson());
    ret.insert("cursorWatch",watch.providerId);
    ret.insert("musicPlayer",musicPlayer.providerId);
    sendMsg(ret,id,true);
}

void KeyFrameScene::clientUnregistered(QJsonObject msg, int id)
{

}

void KeyFrameScene::clientMessage(QJsonObject msg, int id)
{

    if(msg.contains("get_keyframes")){

        QString devId = msg.value("get_keyframes").toString();
        foreach (QSharedPointer<Device> d, myDevs) {
            if(d.data()->getDeviceId() != devId)
                continue;

             QJsonObject ret = getLampJson(d);
             QJsonArray keyframesJson;

             foreach (QSharedPointer<Keyframe> k, keyframes) {
                 if(k.data()->state.deviceId == devId){
                     keyframesJson.append(k.data()->providerId);
                 }
             }
             ret.insert("keyframes",keyframesJson);
             sendMsg(ret,id,true);
             break;
        }
    }
    else if(msg.contains("add_keyframe")){
        QJsonObject ret;
        QJsonObject insertCommand = msg.value("add_keyframe").toObject();
        QString devId = insertCommand.value("devId").toString();
        double time = insertCommand.value("time").toDouble();
        foreach (QSharedPointer<Device> d, myDevs) {
            if(d.data()->getDeviceId() == devId){
                DeviceState state = d.data()->getState();
                QSharedPointer<Keyframe> next;
                foreach (QSharedPointer<Keyframe> frame, keyframes) {
                    if(devId == frame.data()->state.deviceId){
                        if(next.isNull() || qAbs(frame.data()->time - time) < qAbs(next.data()->time - time)){
                            next = frame;
                            state = frame.data()->state;
                        }
                    }
                }

                QSharedPointer<Keyframe> pointer = QSharedPointer<Keyframe>(new Keyframe(time,DeviceState(state),wss));
                connect(pointer.data(),SIGNAL(deleteRequested(Keyframe*)),this,SLOT(removeKeyframe(Keyframe*)));
                keyframes.append(pointer);
                ret.insert("new_keyframe",pointer.data()->providerId);
                ret.insert("devId",devId);
                sendMsg(ret,true);
                break;
            }
        }

    }
    else if(msg.contains("clear")){
        QString devId = msg.value("clear").toString();
        clear(devId);
    }
    else if(msg.contains("copy")){
        QJsonObject copy = msg.value("copy").toObject();
        QString from = copy.value("from").toString();
        QString to = copy.value("to").toString();
        clear(to);

        foreach(QSharedPointer<Device> dev, myDevs){
            if(dev.data()->getDeviceId() == to){
                foreach (QSharedPointer<Keyframe> frame, keyframes) {
                    if(frame.data()->state.deviceId == from){
                        DeviceState state = dev.data()->getState();
                        state.tryImport(frame.data()->state);
                        QSharedPointer<Keyframe> pointer = QSharedPointer<Keyframe>(new Keyframe(frame.data()->time,DeviceState(state),wss));
                        connect(pointer.data(),SIGNAL(deleteRequested(Keyframe*)),this,SLOT(removeKeyframe(Keyframe*)));
                        keyframes.append(pointer);
                        QJsonObject ret;
                        ret.insert("new_keyframe",pointer.data()->providerId);
                        ret.insert("devId",to);
                        sendMsg(ret,true);
                    }
                }
            break;
            }
        }
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

void KeyFrameScene::handleResume()
{
    musicPlayer.play(watch.getMSecs());
}

void KeyFrameScene::handleTimeChanged()
{
    musicPlayer.setPosition(watch.getMSecs());
}

QJsonArray KeyFrameScene::getLampsJson()
{
    QJsonArray ret;
    foreach (QSharedPointer<Device> dev, myDevs) {
        ret.append(getLampJson(dev));
    }
    return ret;
}

QJsonObject KeyFrameScene::getLampJson(QSharedPointer<Device> dev)
{
    QJsonObject ret;
    ret.insert("devID",dev.data()->getDeviceId());
    QJsonObject devChannels;
    if(dev.data()->getType() == Device::RGBW || dev.data()->getType() == Device::RGB){
        devChannels.insert("r",true);
        devChannels.insert("g",true);
        devChannels.insert("b",true);
    }
    if(dev.data()->getType() == Device::RGBW || dev.data()->getType() == Device::White){
        devChannels.insert("w",true);
    }
    ret.insert("channels",devChannels);

    if(dev.data()->getType() == Device::White){//Here we will add the "color" of white devices so it will be displayed correctly
        WhiteDevice *whiteDev = dynamic_cast<WhiteDevice*>(dev.data());
        if(whiteDev != NULL){
            ret.insert("whiteDeviceColor", whiteDev->getColorOfDevice().name());
        }
        else{
            //well this shite device is not of it's class - strange...
            ret.insert("whiteDeviceColor","#ffffff");
        }
    }
    return ret;
}

void KeyFrameScene::clear(QString devId)
{
    for(int i = 0; i < keyframes.length();){
        if(keyframes.at(i).data()->state.deviceId == devId){
            keyframes.at(i).data()->beforeDelete();
            keyframes.removeAt(i);
            i=0;
        }
        i++;
    }
}
