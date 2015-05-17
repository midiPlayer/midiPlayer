#include "beamerdevice.h"

BeamerDevice::BeamerDevice(BeamerDevice::ShutterType shutterTypeP, QJsonObject shutterParamsP) :
    Device(QMap<int,float>(),"",Device::Beamer),shutterType(shutterTypeP),shutterParams(shutterParamsP)
{

}

BeamerDevice::BeamerDevice(QMap<int, float> channelsP, QString devIdP, ShutterType shutterTypeP, QJsonObject shutterParamsP) :
    Device(channelsP,devIdP,Device::Beamer), shutterType(shutterTypeP),shutterParams(shutterParamsP)
{

}

BeamerDevice::BeamerDevice(const BeamerDevice &d) : Device(d), shutterType(d.getShutterType()),
    shutterParams(d.shutterParams)
{

}

BeamerDevice::BeamerDevice(const BeamerDevice *d) : Device(d),shutterType(d->getShutterType()),
    shutterParams(d->shutterParams)
{

}


BeamerDevice::ShutterType BeamerDevice::getShutterType() const
{
 return shutterType;
}

