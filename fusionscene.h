#ifndef FUSIONSCENE_H
#define FUSIONSCENE_H
#include "scene.h"
#include "device.h"
#include <QString>
#include <QList>
class FusionScene : public Scene
{
public:
    FusionScene(QString name);
    void fusion(Scene* with, Device::FusionType fusionType, float opacity);
    void fusion(QList<Device> with, Device::FusionType fusionType, float opacity);
    void reset();
    void import(Scene* import);
    void import(QList<Device> ds);

    QList<Device> getLights();
    QList<Device> getUsedLights();

private:
    QList<Device> devices;
};

#endif // FUSIONSCENE_H
