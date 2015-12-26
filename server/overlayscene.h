#ifndef OVERLAYSCENE_H
#define OVERLAYSCENE_H
#include "scene.h"
#include <QString>
#include "device.h"
#include <QList>
class OverlayScene
{
public:
    OverlayScene(QString nameP, Scene* sceneP,bool min = 0);
    QList<Device> getEffect(QList<Device> other);
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
