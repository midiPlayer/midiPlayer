#ifndef DISCOSCENE_H
#define DISCOSCENE_H

#include "scene.h"
#include "discosubscene.h"
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
private:
    QList<DiscoSubScene> effects;
    QMap<int,Device> availableDevices;
};

#endif // DISCOSCENE_H
