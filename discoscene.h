#ifndef DISCOSCENE_H
#define DISCOSCENE_H

#include "scene.h"
#include <QString>
#include <QList>
#include <QMap>
#include "device.h"
#include "fusionscene.h"
#include "websocketserverprovider.h"
#include "discosubscene.h"
#include <QSharedPointer>
#include "scenebuilder.h"

class DiscoScene : public Scene, public WebSocketServerProvider
{
public:
    DiscoScene(WebSocketServer* ws,SceneBuilder* builderP,QString name,QJsonObject serialized = QJsonObject());
    QList<Device> getLights();
    QList<Device> getUsedLights();
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
    void stop();
    void start();
    QJsonObject serialize();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();
public:
    void addEffect(QSharedPointer<Scene> scene);
private:
    void addSubScene(QSharedPointer<DiscoSubScene> subScene);
    bool solo;
    Scene* soloScene;
    bool isRunning;

    QMap<int,QSharedPointer<DiscoSubScene> > effects;
    QList<int> order;
    int sceneIdCounter;
    FusionScene fusion;
    SceneBuilder* sceneBuilder;
    QJsonObject getStatus(bool showEffects, bool showOrder,bool serializeScene);
};

#endif // DISCOSCENE_H
