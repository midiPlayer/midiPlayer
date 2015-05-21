#include "blackscene.h"

BlackScene::BlackScene(QString name,QList<Device> avDev) : Scene(name),devices(avDev)
{
    foreach (Device d, devices) {
        foreach (int channel, d.getChannels()) {
            d.setChannel(channel,0.0f);
        }
    }
}

BlackScene::BlackScene(QJsonObject serialized,QList<Device> avDev) : Scene(serialized),devices(avDev)
{

}

QList<Device> BlackScene::getUsedLights()
{
    return devices;
}

QJsonObject BlackScene::serialize(QJsonObject inherited)
{
    //nothing to erialize as this is an really trivial scenen
    return Scene::serialize(inherited);
}

QList<Device> BlackScene::getLights()
{
    return devices;
}

