#include "blackscene.h"

BlackScene::BlackScene(QString name,QList<Device> avDev) : Scene(name),devices(avDev)
{
    foreach (Device d, devices) {
        foreach (int channel, d.getChannels()) {
            d.setChannel(channel,0.0f);
        }
    }
}

QList<Device> BlackScene::getUsedLights()
{
    return devices;
}

QList<Device> BlackScene::getLights()
{
    return devices;
}

