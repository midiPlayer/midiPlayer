#ifndef KEYFRAMESCENE_H
#define KEYFRAMESCENE_H
#include "scene.h"
#include <QTime>
#include <QList>
#include "device.h"
#include "keyframe.h"
class KeyFrameScene : public Scene
{
public:
    KeyFrameScene(QList<Device> avDev,QString name, QJsonObject serialized = QJsonObject());
    QList<Device>getLights();
    QList<Device> getUsedLights();
    void start();
    void stop();
    QJsonObject serialize();
private:
    QTime time;
    QList<Device> myDevs;
    QList<Keyframe> keyframes;
};

#endif // KEYFRAMESCENE_H
