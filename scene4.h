#ifndef SCENE4_H
#define SCENE4_H
#include "scene.h"
class Scene4 : public Scene
{
public:
    Scene4(QString name);
    QMap<int,float>getLights();
    QSet<int> getUsedLights();
    int getFadeOutDuration();
};

#endif // SCENE1_H
