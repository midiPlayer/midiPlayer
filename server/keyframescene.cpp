#include "keyframescene.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include "websocketserver.h"
#include "whitedevice.h"

#define KEY_MUSIC_PLAYER "music_player"

#define KEY_KEYFRAMESCENE_KEYFRAMES "key_keyframescene_keyframes"

KeyFrameScene::KeyFrameScene(VirtualDeviceManager *manager, QString name, WebSocketServer *ws, QJsonObject serialized) :
    Scene(name,serialized), WebSocketServerProvider(ws),filterVDevManager(manager), wss(ws),watch(ws),
    musicPlayer(ws,serialized.value(KEY_MUSIC_PLAYER).toObject())
{
    ws->registerProvider(this);

    connect(&watch,SIGNAL(resumed()),this,SLOT(handleResume()));
    connect(&watch,SIGNAL(started()),this,SLOT(handleResume()));
    connect(&watch,SIGNAL(stoped()),&musicPlayer,SLOT(stop()));
    connect(&watch,SIGNAL(timeSet()),this,SLOT(handleTimeChanged()));

    filterVDevManager.addAcceptedType(Device::Beamer);
    filterVDevManager.addAcceptedType(Device::RGB);
    filterVDevManager.addAcceptedType(Device::RGBW);
    filterVDevManager.addAcceptedType(Device::White);

    if(serialized.length() != 0 && serialized.contains(KEY_KEYFRAMESCENE_KEYFRAMES)){
        foreach (QJsonValue value, serialized.value(KEY_KEYFRAMESCENE_KEYFRAMES).toArray()) {
            QSharedPointer<Keyframe> pointer = QSharedPointer<Keyframe>(new Keyframe(value.toObject(),wss,&filterVDevManager));
            connect(pointer.data(),SIGNAL(deleteRequested(Keyframe*)),this,SLOT(removeKeyframe(Keyframe*)));
            keyframes.append(pointer);
        }
    }
}

QMap<QString, QSharedPointer<DeviceState> > KeyFrameScene::getDeviceState()
{
    QMap<QString, QSharedPointer<DeviceState> > ret;

    foreach (QString devId,filterVDevManager.getDevices().keys()) {
        QSharedPointer<Keyframe> min;
        QSharedPointer<Keyframe> max;
        double elapsed = (double)watch.getMSecs() / 1000.0;
        foreach (QSharedPointer<Keyframe> frame, keyframes) {
            if(devId == frame.data()->state.getDevice()->getDeviceId()){
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
            QSharedPointer<DeviceState> ptr = QSharedPointer<DeviceState>(min.data()->fusionWith(max,elapsed).copyToSharedPointer());
            ret.insert(devId,ptr);
        }
        else if(!min.isNull()){
            QSharedPointer<DeviceState> ptr = QSharedPointer<DeviceState>((min.data()->state).copyToSharedPointer());
            ret.insert(devId,ptr);
        }
        else if(!max.isNull()){
            QSharedPointer<DeviceState> ptr = QSharedPointer<DeviceState>(max.data()->state.copyToSharedPointer());
            ret.insert(devId,ptr);
        }
    }

    ret.insert(musicPlayer.getDeviceId(),musicPlayer.createEmptyState());

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
        foreach (QSharedPointer<Device> d, filterVDevManager.getDevices().values()) {
            if(d.data()->getDeviceId() != devId)
                continue;

             QJsonObject ret = getLampJson(d);
             QJsonArray keyframesJson;

             foreach (QSharedPointer<Keyframe> k, keyframes) {
                 if(k.data()->state.getDevice()->getDeviceId() == devId){
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
        foreach (QString did, filterVDevManager.getDevices().keys()) {
            if(did == devId){
                ChannelDevice *d = dynamic_cast<ChannelDevice*>(filterVDevManager.getDevices().value(did).data());
                if(d == NULL)
                    continue;
                ChannelDeviceState state = ChannelDeviceState(d->createEmptyChannelState().data());
                QSharedPointer<Keyframe> next;
                foreach (QSharedPointer<Keyframe> frame, keyframes) {
                    if(devId == frame.data()->state.getDevice()->getDeviceId()){
                        if(next.isNull() || qAbs(frame.data()->time - time) < qAbs(next.data()->time - time)){
                            next = frame;
                            state = frame.data()->state;
                        }
                    }
                }

                QSharedPointer<Keyframe> pointer = QSharedPointer<Keyframe>(new Keyframe(time,ChannelDeviceState(state),wss));
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

        foreach(QString did, filterVDevManager.getDevices().keys()){
            if(did == to){
                ChannelDevice *dev = dynamic_cast<ChannelDevice*>(filterVDevManager.getDevices().value(did).data());
                foreach (QSharedPointer<Keyframe> frame, keyframes) {
                    if(frame.data()->state.getDevice()->getDeviceId() == from){
                        ChannelDeviceState state = ChannelDeviceState(dev->createEmptyChannelState().data());
                        state.tryImport(frame.data()->state);
                        QSharedPointer<Keyframe> pointer = QSharedPointer<Keyframe>(new Keyframe(frame.data()->time,ChannelDeviceState(state),wss));
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

void KeyFrameScene::devicesChanged()
{

}

QJsonArray KeyFrameScene::getLampsJson()
{
    QJsonArray ret;
    foreach (QSharedPointer<Device> dev, filterVDevManager.getDevices().values()) {
        ret.append(getLampJson(dev));
    }
    return ret;
}

QJsonObject KeyFrameScene::getLampJson(QSharedPointer<Device> dev)
{
    QJsonObject ret;
    ret.insert("devID",dev.data()->getDeviceId());
    QJsonObject devChannels;
    if(dev.data()->getType() == Device::RGBW || dev.data()->getType() == Device::RGB || dev.data()->getType() == Device::Beamer){
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
        if(keyframes.at(i).data()->state.getDevice()->getDeviceId() == devId){
            keyframes.at(i).data()->beforeDelete();
            keyframes.removeAt(i);
            i=0;
        }
        i++;
    }
}
