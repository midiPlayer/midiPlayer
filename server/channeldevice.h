#ifndef CHANNELDEVICE_H
#define CHANNELDEVICE_H
#include "device.h"
#include "channeldevicestate.h"
class ChannelDevice : public Device
{
public:
    ChannelDevice(int firstChannelP, int numChannelsP,QString devIdP,DeviceType typeP,QVector3D pos = QVector3D(0,0,0));

    QJsonObject serialize();

    int getNumChannels();
    int getFirstChannel();
    QSharedPointer<DeviceState> createEmptyState();
    QSharedPointer<ChannelDeviceState> createEmptyChannelState();
    static bool istChannelDeviceType(Device::DeviceType type);

private:
    int firstChannel;
    int numChannel;
};

#endif // CHANNELDEVICE_H
