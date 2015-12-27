#include "device.h"
#include <QDebug>
#include "devicestate.h"
Device::Device(QMap<int,float> channelsP, QString devIdP, DeviceType typeP, QVector3D pos) : state(devIdP, channelsP),devId(devIdP),type(typeP),position(pos)
{

}

Device::Device(int firstChannel, int numChannels, QString devIdP, Device::DeviceType typeP, QVector3D pos):
    state(devIdP),devId(devIdP),type(typeP),position(pos)
{
    for(int  i = firstChannel; i <firstChannel+numChannels; i++){
        state.dmxChannels.insert(i,0.0f);
    }
}

Device::Device(const Device &d) : Device(&d)
{

}

Device::Device(const Device *d) : state(d->state),devId(d->devId),type(d->type),position(d->position)
{

}

Device Device::fusionWith(Device upper, DeviceState::FusionType type, float opacity)
{
    if(this->getNumChannels() != upper.getNumChannels() || this->getChannels() != upper.getChannels())
        throw "not compatible";//we can only fusion equal devices
    Device ret(this);
    ret.state = state.fusionWith(upper.state,type,opacity);
    return ret;
}

int Device::getNumChannels()
{
    return state.dmxChannels.count();
}

QMap<int, float> Device::getChannelValues()
{
    return state.dmxChannels;
}

QList<int> Device::getChannels()const{
    return state.getChannels();
}

void Device::setChannel(int channel, float value)
{
    state.setChannel(channel,value);
}

float Device::getChannelValue(int channel)
{
    return state.getChannelValue(channel);
}

QList<Device> Device::loadDevicesFromXml(QString file)
{
    //http://doc.qt.io/qt-5/qtxml-module.html
    /*
     * <device id='' name='' desc=''>
     *  <channel id='1'>
     *  <channel id='2'>
     * </device>
     * *>
     * */

}

bool Device::deviceEqual(Device other)
{
    return deviceEqual(&other);
}

bool Device::deviceEqual(Device *other)
{
    return other->getDeviceId() == getDeviceId();
}

bool Device::valuesEqual(Device other)
{
    return (getChannelValues() == other.getChannelValues());
}

Device Device::findEqualDevice(QList<Device> devices)
{
    foreach (Device d, devices) {
        if(d.deviceEqual(this)){
            return d;
        }
    }
}

QMap<int, float> Device::toLagacy(QList<Device> devices)
{
    QMap<int,float> ret;
    foreach (Device d, devices) {
        QMap<int,float> cValues = d.getChannelValues();
        QMap<int, float>::iterator i;
        for (i = cValues.begin(); i != cValues.end(); ++i){
            if(!ret.contains(i.key()))
            ret.insert(i.key(),i.value());
        }
    }
    return ret;
}

QDebug Device::operator<<(QDebug debug)
{
    debug << "asdf";
    return debug;
}

bool Device::operator==(const Device &other)
{
    return deviceEqual(other);
}

Device Device::operator *(float percentage)
{
    Device ret(this);
    foreach (int channel, getChannels()) {
        ret.setChannel(channel,getChannelValue(channel)*percentage);
    }
    return ret;
}

QString Device::getDeviceId() const
{
    return devId;
}

Device::DeviceType Device::getType()
{
    return type;
}

int Device::getFirstChannel()
{
    return state.getFirstChannel();
}
QVector3D Device::getPosition() const
{
    return position;
}

void Device::setPosition(const QVector3D &value)
{
    position = value;
}

DeviceState Device::getState()
{
    return DeviceState(getDeviceId(),getChannelValues());
}

void Device::setState(DeviceState stateP)
{
    if(stateP.deviceId != getDeviceId())
        throw("wrong state");
    state = stateP;
}


/*
QDebug operator<<(QDebug dbg, Device &type)
{
dbg.nospace() << "Device(" << type.getChannelValues() << ")";
return dbg.maybeSpace();
}
*/


bool operator==(const Device &a,const Device &b)
{
    return a.getDeviceId() == b.getDeviceId();
}

