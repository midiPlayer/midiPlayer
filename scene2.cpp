#include "scene2.h"
#include <ctime>
#include <QDebug>
#include "math.h"
#include <QColor>

Scene2::Scene2(QString name) : Scene(name)
{
}
QMap<int, float> Scene2::getLights()
{
    long dT =  getDeltaTime();
    QMap<int,float> lights;
    //float light1 = sin(dT/1200.0d)*0.3;
    //light1 += sin(dT/500.0d)*0.1 +0.6;
    //light1 += sin(dT/100.0d)*0.05;
    //if(light1 <0) light1*= -1.0f;
    //if(light1 > 1.0f) light1 = 1.0f;
    double x = (0.5 + sin(dT/500.0d)*0.5)*0.5;
    x += (0.5 + sin(dT/100.0d)*0.5)*0.0;
    x += (0.5 + sin(dT/100.0d)*0.5)*0.2;
    x += (0.5 + sin(dT/110.0d)*0.5)*0.2;


    float h = 0.025 + 0.075 * x;
    float v = 0.8 + 0.2 * x;
    QColor a = QColor::fromHsvF(h,1,v);
    lights.insert(1,a.redF());
    lights.insert(2,a.greenF());
    lights.insert(3,a.blueF());

    return lights;
}

QSet<int> Scene2::getUsedLights()
{
    QSet<int> usedLights;
    usedLights << 1;
    usedLights << 2;
    usedLights << 3;
    return usedLights;
}

int Scene2::getFadeOutDuration()
{
    return 3000;
}
