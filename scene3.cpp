#include "scene3.h"
#include "math.h"
#include <QDebug>
Scene3::Scene3()
{
}

QMap<int, float> Scene3::getLights()
{
    long dT =  getDeltaTime();
    QMap<int,float> lights;
    float light1 = sin(dT/100.0d);
    qDebug() << light1;
    if(light1 > 0.5) light1 = 1; else light1 = 0;
    lights.insert(1,light1);
    qDebug() << light1;
    return lights;
}

QSet<int> Scene3::getUsedLights()
{
    QSet<int> usedLights;
    usedLights << 1;
    return usedLights;
}

int Scene3::getFadeOutDuration()
{
    return 3000;
}
