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

class DiscoScene : public Scene, public WebSocketServerProvider
{
public:
    DiscoScene(QString name,WebSocketServer* ws);
    QList<Device> getLights();
    QList<Device> getUsedLights();
    QString getSceneTypeString();
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
    void stop();
    void start();
    QJsonObject serialize();
public:
    void addEffect(QSharedPointer<Scene> scene);
private:
    bool solo;
    Scene* soloScene;


    QMap<int,QSharedPointer<DiscoSubScene> > effects;
    QList<int> order;
    int sceneIdCounter;
    FusionScene fusion;

    QJsonObject getStatus(bool showEffects, bool showOrder,bool serialize);
};

#endif // DISCOSCENE_H
