#ifndef FUSIONSCENE_H
#define FUSIONSCENE_H
#include "scene.h"
#include "device.h"
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QSharedPointer>
class FusionScene : public Scene
{
public:
    FusionScene(QString name);
    void fusion(QSharedPointer<Scene> with, Device::FusionType fusionType, float opacity);
    void fusion(Scene *with, Device::FusionType fusionType, float opacity);
    void fusion(QList<Device> with, Device::FusionType fusionType, float opacity);
    QList<Device> passiveFusion(QList<Device> with, Device::FusionType fusionType, float opacity);
    void reset();
    void import(Scene* import);
    void import(QList<Device> ds);
    void import(QSharedPointer<Scene> import);

    QList<Device> getLights();
    QList<Device> getUsedLights();

    QJsonObject serialize();
    QString getSceneTypeString();

private:
    QList<Device> devices;
};

#endif // FUSIONSCENE_H
