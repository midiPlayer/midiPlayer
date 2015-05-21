#ifndef DISCOSUBSCENE_H
#define DISCOSUBSCENE_H

#include <QJsonObject>
#include <serializable.h>
#include "scene.h"
class DiscoSubScene : public Serializable
{
public:
    DiscoSubScene(int idP, Scene* sceneP, Device::FusionType fusionTypeP, bool muteP = false, float opacityP = 1.0);
    int id;
    Scene* scene;
    bool mute;
    float opacity;
    Device::FusionType fusionType;
    QJsonObject getJsonForClient();
    QJsonObject serialize();
private:
    QJsonObject getBasicJson();
};

#endif // DISCOSUBSCENE_H
