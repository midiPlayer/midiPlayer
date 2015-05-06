#include "fusionscene.h"

FusionScene::FusionScene(QString name) : Scene(name)
{

}

void FusionScene::fusion(Scene *with, Device::FusionType fusionType,float opacity)
{
    fusion(with->getLights(),fusionType,opacity);
}

void FusionScene::fusion(QList<Device> with, Device::FusionType fusionType, float opacity)
{
    foreach(Device device,with){
        if(devices.contains(device))
            device.findEqualDevice(devices).fusionWith(device,fusionType,opacity);
        else
            devices.append(device);
    }
}

void FusionScene::reset()
{
    devices.clear();
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

