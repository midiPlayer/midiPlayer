#ifndef CHANNELDEVICESTATE_H
#define CHANNELDEVICESTATE_H
#include <QMap>
#include "serializable.h"
#include "devicestate.h"
#include "virtualdevicemanager.h"
#include <exception>
class ChannelDevice;

class ChannelDeviceState : public Serializable, public DeviceState
{

public:

    ChannelDeviceState(ChannelDevice *deviceP);
    ChannelDeviceState(const ChannelDeviceState &state);
    ChannelDeviceState(ChannelDeviceState *state);
    ChannelDeviceState(QJsonObject serialized,VirtualDeviceManager *manager);
    ChannelDevice *device;
    QMap<int,float> dmxChannels;
    QSharedPointer<DeviceState> fusionWith(QSharedPointer<DeviceState> upperDeviceState, FusionType type, float opacity);
    QSharedPointer<DeviceState> fusionAlone(FusionType type, float opacity);

    void setChannel(int channel,float value);
    float getChannelValue(int channel);
    QList<int> getChannels() const;
    int getNumChannels();
    int getFirstChannel();
    QJsonObject serialize();
    QJsonArray getClientJson();
    void setClientJson(QJsonArray json);
    ChannelDeviceState operator *(float percentage);
    void multiply(float percentage);

    ChannelDeviceState copy();
    QSharedPointer<ChannelDeviceState> copyToSharedPointer();

    bool equal(DeviceState *other);

    void tryImport(ChannelDeviceState other);

    ChannelDevice* getDevice();
};

#endif // DEVICESTATE_H
