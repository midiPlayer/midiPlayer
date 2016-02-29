#ifndef DISCOSUBSCENE_H
#define DISCOSUBSCENE_H

#include <QJsonObject>
#include <serializable.h>
#include "scene.h"
#include <QSharedPointer>
#include "scenebuilder.h"
#include "channeldevicestate.h"
class DiscoSubScene
{
public:
    DiscoSubScene(int idP, QSharedPointer<Scene>sceneP, ChannelDeviceState::FusionType fusionTypeP, bool muteP = false, float opacityP = 1.0);
    DiscoSubScene(QJsonObject serialized, SceneBuilder *sceneBuilder, int idP);
    int id;
    QSharedPointer<Scene> scene;
    bool mute;
    float opacity;
    ChannelDeviceState::FusionType fusionType;
    QJsonObject getJsonForClient();
    QJsonObject serialize(SceneBuilder *builder);
    void setFusinType(QString fusionTypeStr);
private:
    QJsonObject getBasicJson();
};

#endif // DISCOSUBSCENE_H
