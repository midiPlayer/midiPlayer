#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include "scene.h"
class SceneBuilder
{
public:
    SceneBuilder();
    Scene* build(QString sceneType, QString name);
    Scene* build(QString sceneType, QJsonObject serialied);
};

#endif // SCENEBUILDER_H
