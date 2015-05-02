#include "discoscene.h"


DiscoScene::DiscoScene(name) : Scene(name),effects(),availableDevices()
{
    QMap lights;
    lights.insert(1,0.0);
    lights.insert(2,0.0);
    lights.insert(3,0.0);
    availableDevices.insert(1,new Device(lights));
}



QMap<int, float> DiscoScene::getLights()
{
    foreach (DiscoSubScene effect, effects) {

    }
}

QMap<int, Device> DiscoScene::getAvailableDevices()
{
    return availableDevices;
}
