#include "beamershutterscenemanager.h"
#include "websocketserver.h"
#include <QJsonArray>
BeamerShutterSceneManager::BeamerShutterSceneManager(beamerDeviceProvider *provider,WebSocketServer *ws,JackProcessor *p) : shutterScenes(), shutterScenesIdCounter(0),currentScene(-1),
    circularBeamerShutterScene("Circular",provider,ws,p),WebSocketServerProvider(ws)
{
    addScene(&circularBeamerShutterScene,true);
    ws->registerProvider(this);
}

void BeamerShutterSceneManager::addScene(BeamerShutterScene *scene,bool activate)
{
    shutterScenes.insert(shutterScenesIdCounter,scene);
    if(activate)
        setCurrentScene(shutterScenesIdCounter);
    shutterScenesIdCounter++;
}

void BeamerShutterSceneManager::clientRegistered(QJsonObject msg, int clientId)
{
    sendMsg(getStatus(),clientId);
}

void BeamerShutterSceneManager::clientUnregistered(QJsonObject msg, int clientId)
{

}

void BeamerShutterSceneManager::clientMessage(QJsonObject msg, int clientId)
{

}

QString BeamerShutterSceneManager::getRequestType()
{
   return "BeamerShutterControl";
}

void BeamerShutterSceneManager::setCurrentScene(int id)
{
    if(currentScene == id)
        return;
    if(currentScene != -1)
        shutterScenes.value(currentScene)->stop();
    currentScene = id;
    shutterScenes.value(currentScene)->start();
}

QJsonObject BeamerShutterSceneManager::getSceneStatus(int sceneId)
{
    QJsonObject ret;
    ret.insert("sceneId",sceneId);
    BeamerShutterScene* scene = shutterScenes.value(sceneId);
    ret.insert("requestId",scene->providerId);
    ret.insert("requestType",scene->getRequestType());
    ret.insert("name",scene->getName());
    ret.insert("isAcitve",currentScene == sceneId);
    return ret;
}

QJsonObject BeamerShutterSceneManager::getStatus()
{
    QJsonObject ret;
    QJsonArray scenesObj;
    foreach(int sceneID,shutterScenes.keys())
        scenesObj.append(getSceneStatus(sceneID));

    ret.insert("effectList",scenesObj);
    return ret;
}

