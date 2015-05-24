#include "colorscene.h"
#include <QListIterator>

void ColorScene::init()
{
    QList<Device>::iterator deviceIter = devices.begin();
    while(deviceIter != devices.end()){
        Device d = *deviceIter;
        foreach (int channel, d.getChannels()) {
            (*deviceIter).setChannel(channel,1.0f);
        }
        deviceIter++;
    }
}

ColorScene::ColorScene(QString name,QList<Device> avDev) : Scene(name),devices(avDev)
{
    init();
}

ColorScene::ColorScene(QJsonObject serialized,QList<Device> avDev) : Scene(serialized),devices(avDev)
{
    init();
}

QList<Device> ColorScene::getUsedLights()
{
    return devices;
}

QJsonObject ColorScene::serialize(QJsonObject inherited)
{
    //nothing to erialize as this is an really trivial scenen
    return Scene::serialize(inherited);
}

QString ColorScene::getSceneTypeString()
{
    return "blackscene";
}

QList<Device> ColorScene::getLights()
{
    return devices;
}

