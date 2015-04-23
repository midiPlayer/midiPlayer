#include "scene1.h"

Scene1::Scene1(QString nameP) : Scene(nameP)
{

}

QMap<int, float> Scene1::getLights()
{
    QMap<int,float> lights;
    lights.insert(1,0.5);
    lights.insert(2,0.8);
    return lights;
}

QSet<int> Scene1::getUsedLights()
{
    QSet<int> usedLights;
    usedLights << 1;
    usedLights << 2;
    return usedLights;
}

int Scene1::getFadeOutDuration()
{
    return 5000;
}
