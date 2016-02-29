#include "fusionscene.h"
#include <QDebug>


FusionScene::FusionScene(QString name,QJsonObject serialized) : Scene(name,serialized)
{

}

void FusionScene::fusion(Scene *with, ChannelDeviceState::FusionType fusionType,float opacity)
{
    fusion(with->getDeviceState(),fusionType,opacity);
}

void FusionScene::fusion(QSharedPointer<Scene> with, ChannelDeviceState::FusionType fusionType,float opacity)
{
    fusion(with.data()->getDeviceState(),fusionType,opacity);
}

void FusionScene::fusion(QMap<QString, QSharedPointer<DeviceState> >  with, ChannelDeviceState::FusionType fusionType, float opacity)
{
    deviceStates = passiveFusion(with,fusionType,opacity);
}

QMap<QString, QSharedPointer<DeviceState> > FusionScene::passiveFusion(QMap<QString, QSharedPointer<DeviceState> > with, ChannelDeviceState::FusionType fusionType, float opacity)
{
    QMap<QString, QSharedPointer<DeviceState> > ret;
    foreach(QString deviceKey,with.keys()){
        if(deviceStates.contains(deviceKey)){
            QSharedPointer<DeviceState> my = deviceStates.value(deviceKey);
                ret.remove(deviceKey);
                ret.insert(deviceKey,my.data()->fusionWith(with.value(deviceKey),fusionType,opacity));
        }
        else
            ret.insert(deviceKey,with.value(deviceKey).data()->fusionAlone(fusionType,1-opacity));
    }
    foreach (QString deviceKey, deviceStates.keys()) {
        if(!ret.contains(deviceKey))
            ret.insert(deviceKey,deviceStates.value(deviceKey).data()->fusionAlone(fusionType,opacity));
    }
    return ret;
}

void FusionScene::reset()
{
    deviceStates.clear();
}

void FusionScene::import(QSharedPointer<Scene> importScene)
{
    import(importScene.data());
}

void FusionScene::import(QMap<QString, QSharedPointer<DeviceState> > imp)
{
    deviceStates = imp;
}

QMap<QString, QSharedPointer<DeviceState> > FusionScene::getDeviceState()
{
    return deviceStates;
}

void FusionScene::import(Scene *import)
{
    deviceStates = import->getDeviceState();
}


QJsonObject FusionScene::serialize()
{
    QJsonObject ret;//it is useless to serialize a fusion scene!
    return serializeScene(ret);
}

QString FusionScene::getSceneTypeString()
{
    return "fusionScene";
}

