#ifndef KEYFRAMESCENE_H
#define KEYFRAMESCENE_H
#include "scene.h"
class KeyFrameScene : public Scene
{
public:
    KeyFrameScene(QMap<int, QMap<int, float> > frames, QString name,bool autoExitP, QString desc = 0);
    KeyFrameScene(QString path, QString name,bool autoExitP, QString desc = 0);
    QMap<int,float>getLights();
    QSet<int> getUsedLights();
    int getFadeOutDuration();
    bool exitRequested();
    void start();
private:
    QMap<int,QMap<int,float> > keyframes;
    QMap<int, float> keyFrameFusion(QMap<int, float> sceneA, QMap<int, float> sceneB, float ballance,QSet<int> usedLamps);
    QSet<int> usedLamps;
    bool exitRequestedV;
    bool autoExit;
};

#endif // KEYFRAMESCENE_H
