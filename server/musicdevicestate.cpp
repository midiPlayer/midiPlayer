#include "musicdevicestate.h"
#include "musicdevice.h"

MusicDeviceState::MusicDeviceState() : devices()
{

}

MusicDeviceState::MusicDeviceState(MusicDevice *parent) : devices()
{
    devices.insert(parent,1.0);
}

QSharedPointer<DeviceState> MusicDeviceState::fusionWith(QSharedPointer<DeviceState> upperDeviceState, DeviceState::FusionType type, float opacity)
{
    QSharedPointer<MusicDeviceState> ret = QSharedPointer<MusicDeviceState>(new MusicDeviceState());

    QSharedPointer<MusicDeviceState> upper = upperDeviceState.dynamicCast<MusicDeviceState>();

    float factorA = 1.0;
    float factorB = 1.0;

    if(type == DeviceState::AV){
        factorA = 1- opacity;
        factorB = opacity;
    }
    else if(type == DeviceState::OVERRIDE){
        factorB = 0;
    }

    foreach(MusicDevice* dev, devices.keys()){
        ret.data()->devices.insert(dev,devices.value(dev) * factorA);
    }
    foreach(MusicDevice* dev, upper.data()->devices.keys()){
        ret.data()->devices.insert(dev,upper.data()->devices.value(dev) * factorB);
    }

    return ret;
}

QSharedPointer<DeviceState> MusicDeviceState::fusionAlone(DeviceState::FusionType type, float opacity)
{
    QSharedPointer<MusicDeviceState> ret = QSharedPointer<MusicDeviceState>(new MusicDeviceState());

    float factorA = 1.0;

    if(type == DeviceState::AV){
        factorA = 1- opacity;
    }

    foreach(MusicDevice* dev, devices.keys()){
        ret.data()->devices.insert(dev,devices.value(dev) * factorA);
    }

    return ret;
}

bool MusicDeviceState::equal(DeviceState *other)
{
    MusicDeviceState *otherMusicState = dynamic_cast<MusicDeviceState*>(other);
    if(otherMusicState == NULL)
        return false;
    return (devices == otherMusicState->devices);
}

void MusicDeviceState::publish()
{
    foreach (MusicDevice* dev, devices.keys()) {
        dev->setVolume(devices.value(dev));
    }
}
