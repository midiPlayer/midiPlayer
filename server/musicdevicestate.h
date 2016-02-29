#ifndef MUSICDEVICESTATE_H
#define MUSICDEVICESTATE_H
#include "devicestate.h"
#include <QMap>
class MusicDevice;
class MusicDeviceState : public DeviceState
{
public:
    MusicDeviceState();
    MusicDeviceState(MusicDevice *parent);
    QSharedPointer<DeviceState> fusionWith(QSharedPointer<DeviceState> upperDeviceState, FusionType type, float opacity);
    QSharedPointer<DeviceState> fusionAlone(FusionType type, float opacity);
    virtual bool equal(DeviceState *other);
    void publish();
private:
    QMap<MusicDevice*,float> devices;
};

#endif // MUSICDEVICESTATE_H
