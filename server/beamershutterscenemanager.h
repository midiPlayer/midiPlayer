#ifndef BEAMERSHUTTERSCENEMANAGER_H
#define BEAMERSHUTTERSCENEMANAGER_H

#include <QMap>
#include "beamershutterscene.h"
#include "circularbeamershutterscene.h"
#include "websocketserverprovider.h"
class BeamerShutterSceneManager : WebSocketServerProvider
{
public:
    BeamerShutterSceneManager(beamerDeviceProvider *provider, WebSocketServer* ws,JackProcessor* p);
    void addScene(BeamerShutterScene *scene, bool activate  =false);
    void clientRegistered(QJsonObject msg,int clientId);
    void clientUnregistered(QJsonObject msg, int clientId);
    void clientMessage(QJsonObject msg,int clientId);
    QString getRequestType();
private:
    QMap<int,BeamerShutterScene*> shutterScenes;
    int currentScene;
    int shutterScenesIdCounter;
    void setCurrentScene(int id);
    QJsonObject getSceneStatus(int sceneId);
    QJsonObject getStatus();
    CircularBeamerShutterScene circularBeamerShutterScene;
};

#endif // BEAMERSHUTTERSCENEMANAGER_H
