#include "beamershutterscenemanager.h"

BeamerShutterSceneManager::BeamerShutterSceneManager(beamerDeviceProvider *provider,WebSocketServer *ws,JackProcessor *p) : shutterScenes(), shutterScenesIdCounter(0),currentScene(-1),
    circularBeamerShutterScene(provider,ws,p)
{
    addScene(&circularBeamerShutterScene,true);
}

void BeamerShutterSceneManager::addScene(BeamerShutterScene *scene,bool activate)
{
    shutterScenes.insert(shutterScenesIdCounter,scene);
    if(activate)
        setCurrentScene(shutterScenesIdCounter);
    shutterScenesIdCounter++;
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

