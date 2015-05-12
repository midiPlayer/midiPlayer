#ifndef DISCOSCENE_H
#define DISCOSCENE_H

#include "scene.h"
#include <QString>
#include <QList>
#include <QMap>
#include "device.h"
#include "fusionscene.h"
#include "websocketserverprovider.h"

class DiscoScene : public Scene, public WebSocketServerProvider
{
public:
    DiscoScene(QString name,WebSocketServer *ws);
    QList<Device> getLights();
    QList<Device> getUsedLights();

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
    void stop();
    void start();
public:
    void addEffect(Scene *scene);
private:
    struct DiscoSubScene{
        int id;
        Scene* scene;
        bool mute;
        float opacity;
        Device::FusionType fusionType;
    };

    bool solo;
    Scene* soloScene;


    QMap<int,DiscoSubScene*> effects;
    QList<int> order;
    int sceneIdCounter;
    FusionScene fusion;

    QJsonObject getStatus(bool showEffects, bool showOrder);
    QJsonObject getEffectJson(DiscoSubScene *effect);
};

#endif // DISCOSCENE_H
