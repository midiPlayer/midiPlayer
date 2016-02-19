#ifndef DIASCENE_H
#define DIASCENE_H

#include "scene.h"
#include "websocketserverprovider.h"
#include "fusionscene.h"
#include <QTime>
#include "jackprocessor.h"
#include "dia.h"
#include "monitorio.h"

class DiaScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT

public:
    DiaScene(QList<Device> avDev, WebSocketServer *ws, JackProcessor *jackP, SceneBuilder *builder,
             MonitorIO* monitorIoP, QString name, QJsonObject serialized = QJsonObject());
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
    void music();
private:
    QList<Device> availableDevices;

    QJsonObject getState(bool addScenes);

    QMap<int, QSharedPointer<Dia> > scenes;
    QList<int> order;
    int current;
    int fadingTo;

    bool nextOnMusic;
    QTime fadeTimer;
    FusionScene fusion;
    WebSocketServer *wss;
    SceneBuilder *builder;

    JackProcessor *jack;

    void next();

    void setNextOnMusic(bool enable);

    MonitorIO *monitorIo;

    int sceneIdCountr;

};

#endif // DIASCENE_H
