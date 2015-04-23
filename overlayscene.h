#ifndef OVERLAYSCENE_H
#define OVERLAYSCENE_H
#include "scene.h"
#include <QString>
class OverlayScene
{
public:
    OverlayScene(QString nameP, Scene* sceneP,bool min = 0);
    QMap<int, float> getEffect(QMap<int, float> other);
    void reset();
    void stop();
    QString getName();
    bool stopRequested();
private:
    QString name;
    Scene* scene;
    bool min;
};

#endif // OVERLAYSCENE_H
