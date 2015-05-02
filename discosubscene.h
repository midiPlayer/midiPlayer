#ifndef DISCOSUBSCENE_H
#define DISCOSUBSCENE_H

#include <scene.h>
#include <QList>
#include "device.h"
class DiscoSubScene : public Scene
{
public:
    DiscoSubScene();
private:
    QList<Device> devices;

};

#endif // DISCOSUBSCENE_H
