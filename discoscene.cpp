#include "discoscene.h"


DiscoScene::DiscoScene(QString name) : Scene(name),availableDevices()
{
    QMap<int,float> lights;
    lights.insert(1,0.0);
    lights.insert(2,0.0);
    lights.insert(3,0.0);
    availableDevices.insert(1,Device(lights));
}



QMap<int, float> DiscoScene::getLights()
{
    foreach (Scene* effect, effects) {

    }
}

QSet<int> DiscoScene::getUsedLights()
{

}

int DiscoScene::getFadeOutDuration()
{

}

QMap<int, Device> DiscoScene::getAvailableDevices()
{
    return availableDevices;
}
