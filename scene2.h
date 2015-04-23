#ifndef SCENE2_H
#define SCENE2_H
#include "scene.h"
class Scene2 : public Scene
{
public:
    Scene2(QString name);
    QMap<int,float>getLights();
    QSet<int> getUsedLights();
    int getFadeOutDuration();
};

#endif // SCENE2_H
