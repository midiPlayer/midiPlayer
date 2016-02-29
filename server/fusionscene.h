#ifndef FUSIONSCENE_H
#define FUSIONSCENE_H
#include "scene.h"
#include "device.h"
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QSharedPointer>
#include "channeldevicestate.h"

class FusionScene : public Scene
{
public:
    FusionScene(QString name,QJsonObject serialized = QJsonObject());
    void fusion(QSharedPointer<Scene> with, ChannelDeviceState::FusionType fusionType, float opacity);
    void fusion(Scene *with, ChannelDeviceState::FusionType fusionType, float opacity);
    void fusion(QMap<QString, QSharedPointer<DeviceState> > with, ChannelDeviceState::FusionType fusionType, float opacity);
    QMap<QString, QSharedPointer<DeviceState> > passiveFusion(QMap<QString, QSharedPointer<DeviceState> > with, ChannelDeviceState::FusionType fusionType, float opacity);
    void reset();
    void import(Scene* import);
    void import(QSharedPointer<Scene> importScene);
    void import(QMap<QString, QSharedPointer<DeviceState> > imp);

    QMap<QString,QSharedPointer<DeviceState> > getDeviceState();

    QJsonObject serialize();
    QString getSceneTypeString();

private:
    QMap<QString, QSharedPointer<DeviceState> > deviceStates;
};

#endif // FUSIONSCENE_H
