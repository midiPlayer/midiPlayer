#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include <QSharedPointer>
#include <websocketserver.h>
#include "jackprocessor.h"
#include "scene.h"
#include "virtualdevicemanager.h"
class Scene;
class SceneBuilder
{
public:
    SceneBuilder(WebSocketServer *wssP,VirtualDeviceManager *manager, JackProcessor *jackP);
    QSharedPointer<Scene> build(QString sceneType, QString name = "", QJsonObject serialized = QJsonObject());
    QSharedPointer<Scene> build(QJsonObject serialized);
    QJsonObject serializeScene(QSharedPointer<Scene> scene);
    QJsonObject serializeScene(Scene* scene);
    QJsonObject serializeScene(Scene *scene,QJsonObject serialized);
private:
    WebSocketServer* wss;
    VirtualDeviceManager *vDevManager;
    JackProcessor *jack;
};

#endif // SCENEBUILDER_H
