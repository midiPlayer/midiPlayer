#include "scene4.h"

Scene4::Scene4(QString name) : Scene(name)
{

}

QMap<int, float> Scene4::getLights()
{
    QMap<int,float> lights;
    return lights;
}

QSet<int> Scene4::getUsedLights()
{
    QSet<int> usedLights;
    return usedLights;
}

int Scene4::getFadeOutDuration()
{
    return 1000;
}
