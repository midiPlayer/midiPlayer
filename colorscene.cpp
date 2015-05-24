#include "colorscene.h"
#include <QListIterator>



ColorScene::ColorScene(QList<Device> avDev, QString name, QJsonObject serialized) : Scene(name,serialized),devices(avDev)
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
    return getSceneTypeStringStaticaly();
}

QString ColorScene::getSceneTypeStringStaticaly()
{
    return "blackscene";
}

QList<Device> ColorScene::getLights()
{
    return devices;
}

