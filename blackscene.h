#ifndef BLACKSCENE_H
#define BLACKSCENE_H
#include "scene.h"

class BlackScene : public Scene
{
public:
    BlackScene(QString name, QList<Device> avDev);
    QList<Device> getLights();
    QList<Device> getUsedLights();
private:
    QList<Device> devices;
};

#endif // BLACKSCENE_H
