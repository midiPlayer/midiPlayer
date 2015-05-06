#ifndef DISCOSCENE_H
#define DISCOSCENE_H

#include "scene.h"
#include <QString>
#include <QList>
#include <QMap>
#include "device.h"
#include "fusionscene.h"

class DiscoScene : public Scene
{
public:
    DiscoScene(QString name);
    QList<Device> getLights();
    QList<Device> getUsedLights();
    int getFadeOutDuration();
private:
    QList<Scene*> effects;
    FusionScene fusion;
};

#endif // DISCOSCENE_H
