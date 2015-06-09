#ifndef DIASCENE_H
#define DIASCENE_H

#include "scene.h"
#include "websocketserverprovider.h"
#include "fusionscene.h"
#include <QTime>
#include "jackprocessor.h"
#include "dia.h"

class DiaScene : public Scene, public WebSocketServerProvider
{
public:
    DiaScene(QList<Device> avDev, WebSocketServer *ws, JackProcessor *jackP, SceneBuilder builder,
             QString name, QJsonObject serialized = QJsonObject());
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

    void addScene(QSharedPointer<Scene>scene, QString name, QString desc, float fadeInDuration);
    void addScene(QSharedPointer<Dia>dia);
    void loadSerialized(QJsonObject serialized);
public slots:
    void triggered();
private:
    QList<Device> availableDevices;

    QJsonObject getState(bool addScenes);

    QList<QSharedPointer<Dia> > scenes;
    int current;
    int fadingTo;

    bool nextOnMusic;
    QTime fadeTimer;
    FusionScene fusion;
    WebSocketServer *wss;
    SceneBuilder builder;

};

#endif // DIASCENE_H
