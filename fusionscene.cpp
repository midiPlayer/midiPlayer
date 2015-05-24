#include "fusionscene.h"
#include <QDebug>


FusionScene::FusionScene(QString name) : Scene(name)
{

}

void FusionScene::fusion(Scene *with, Device::FusionType fusionType,float opacity)
{
    fusion(with->getLights(),fusionType,opacity);
}

void FusionScene::fusion(QSharedPointer<Scene> with, Device::FusionType fusionType,float opacity)
{
    fusion(with.data()->getLights(),fusionType,opacity);
}

void FusionScene::fusion(QList<Device> with, Device::FusionType fusionType, float opacity)
{
    devices = passiveFusion(with,fusionType,opacity);
}

QList<Device> FusionScene::passiveFusion(QList<Device> with, Device::FusionType fusionType, float opacity)
{
    QList<Device> ret;
    foreach(Device device,with){
        if(devices.contains(device)){
            Device my = device.findEqualDevice(devices);
                ret.removeAll(my);
                ret.append(my.fusionWith(device,fusionType,opacity));

        }
        else
            ret.append(device);
    }
    return ret;
}

void FusionScene::reset()
{
    devices.clear();
}

void FusionScene::import(QSharedPointer<Scene> import)
{
    devices = import.data()->getLights();
}

void FusionScene::import(Scene *import)
{
    devices = import->getLights();
}

void FusionScene::import(QList<Device> ds)
{
    devices = ds;
}

QList<Device> FusionScene::getLights()
{
    return devices;
}

QList<Device> FusionScene::getUsedLights()
{
    return devices;
}

QJsonObject FusionScene::serialize()
{
    QJsonObject ret;//it is useless to serialize a fusion scene!
    return ret;
}

QString FusionScene::getSceneTypeString()
{
    return "fusionScene";
}

