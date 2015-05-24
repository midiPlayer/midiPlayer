#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include "scene.h"
#include <QSharedPointer>
#include <websocketserver.h>
#include "jackprocessor.h"
class SceneBuilder
{
public:
    SceneBuilder(WebSocketServer *wssP,QList<Device> avDev,JackProcessor *jackP);
    QSharedPointer<Scene> build(QString sceneType, QString name);
    QSharedPointer<Scene> build(QString sceneType, QJsonObject serialied);
private:
    WebSocketServer* wss;
    QList<Device> availableDevices;
    JackProcessor *jack;
};

#endif // SCENEBUILDER_H
