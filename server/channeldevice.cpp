#include "channeldevice.h"

#define KEY_FIRST_CHANNEL_ID "device_first_channel"
#define KEY_NUM_CHANNEL "device_num_channel"


ChannelDevice::ChannelDevice(int firstChannelP, int numChannelsP, QString devIdP,DeviceType typeP,
                             QVector3D pos) :  Device(devIdP,typeP,pos), firstChannel(firstChannelP),
    numChannel(numChannelsP)
{

}

QJsonObject ChannelDevice::serialize()
{
 QJsonObject ret = Device::serialize();
 ret.insert(KEY_FIRST_CHANNEL_ID,getFirstChannel());
 ret.insert(KEY_NUM_CHANNEL,getNumChannels());
 return ret;
}

int ChannelDevice::getNumChannels()
{
    return numChannel;
}

int ChannelDevice::getFirstChannel()
{
    return firstChannel;
}

QSharedPointer<DeviceState> ChannelDevice::createEmptyState()
{
    return createEmptyChannelState();
}

QSharedPointer<ChannelDeviceState> ChannelDevice::createEmptyChannelState()
{
    QSharedPointer<ChannelDeviceState> ret(new ChannelDeviceState(this));
    return ret;
}

bool ChannelDevice::istChannelDeviceType(Device::DeviceType type)
{
    return (type == Device::Beamer || type == Device::RGB || type == Device::RGBW || type == Device::White);
}
