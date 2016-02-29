#include "beamerdevice.h"

BeamerDevice::BeamerDevice(BeamerDevice::ShutterType shutterTypeP, QJsonObject shutterParamsP) :
    ChannelDevice(0,0,"",Device::Beamer),shutterType(shutterTypeP),shutterParams(shutterParamsP)
{

}

BeamerDevice::BeamerDevice(int firstChannelP, int numChannelsP, QString devIdP, ShutterType shutterTypeP, QJsonObject shutterParamsP) :
    ChannelDevice(firstChannelP,numChannelsP,devIdP,Device::Beamer), shutterType(shutterTypeP),shutterParams(shutterParamsP)
{

}


BeamerDevice::ShutterType BeamerDevice::getShutterType() const
{
 return shutterType;
}

