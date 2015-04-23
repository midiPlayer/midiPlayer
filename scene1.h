#ifndef SCENE1_H
#define SCENE1_H
#include "scene.h"
class Scene1 : public Scene
{
public:
    Scene1(QString nameP);
    QMap<int,float>getLights();
    QSet<int> getUsedLights();
    int getFadeOutDuration();

};

#endif // SCENE1_H
