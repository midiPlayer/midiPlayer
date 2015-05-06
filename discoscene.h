#ifndef DISCOSCENE_H
#define DISCOSCENE_H

#include "scene.h"
#include <QString>
#include <QList>
#include <QMap>
#include "device.h"

class DiscoScene : public Scene
{
public:
    DiscoScene(QString name);
    QMap<int,float>getLights();
    QSet<int> getUsedLights();
    int getFadeOutDuration();
    QMap<int,Device> getAvailableDevices();
private:
    QList<Scene*> effects;
    QMap<int,Device> availableDevices;
};

#endif // DISCOSCENE_H
