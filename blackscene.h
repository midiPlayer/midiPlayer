#ifndef BLACKSCENE_H
#define BLACKSCENE_H
#include "scene.h"
#include <QJsonObject>

class BlackScene : public Scene
{
public:
    BlackScene(QString name, QList<Device> avDev);
    BlackScene(QJsonObject serialized, QList<Device> avDev);
    QList<Device> getLights();
    QList<Device> getUsedLights();
    QJsonObject serialize(QJsonObject inherited = QJsonObject());
    QString getSceneTypeString();
private:
    QList<Device> devices;
};

#endif // BLACKSCENE_H
