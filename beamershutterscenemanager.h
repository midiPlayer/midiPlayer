#ifndef BEAMERSHUTTERSCENEMANAGER_H
#define BEAMERSHUTTERSCENEMANAGER_H

#include <QMap>
#include "beamershutterscene.h"
#include "circularbeamershutterscene.h"
class BeamerShutterSceneManager
{
public:
    BeamerShutterSceneManager(beamerDeviceProvider *provider, WebSocketServer *ws,JackProcessor *p);
    void addScene(BeamerShutterScene *scene, bool activate  =false);
private:
    QMap<int,BeamerShutterScene*> shutterScenes;
    int currentScene;
    int shutterScenesIdCounter;
    void setCurrentScene(int id);

    CircularBeamerShutterScene circularBeamerShutterScene;
};

#endif // BEAMERSHUTTERSCENEMANAGER_H
