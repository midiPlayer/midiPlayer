#ifndef DEVICESTATE_H
#define DEVICESTATE_H
#include <QSharedPointer>

class Device;

class DeviceState
{
public:
    DeviceState();
    enum FusionType { AV,MAX,MAXG,MIN,MING,OVERRIDE};
    virtual QSharedPointer<DeviceState> fusionWith(QSharedPointer<DeviceState> upper, FusionType type, float opacity) = 0;
    virtual QSharedPointer<DeviceState> fusionAlone(FusionType type, float opacity) = 0;
    virtual bool equal(DeviceState *other) = 0;
    virtual void publish() {};
};

#endif // DEVICESTATE_H
