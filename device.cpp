#include "device.h"
#include <QDebug>
Device::Device(QMap<int,float> channelsP, QString devIdP) : dmxChannels(channelsP),devId(devIdP)
{

}

Device::Device(const Device &d) : dmxChannels(d.dmxChannels),devId(d.devId)
{

}

Device::Device(const Device *d) : dmxChannels(d->dmxChannels),devId(d->devId)
{

}

Device Device::fusionWith(Device upper, Device::FusionType type, float opacity)
{
    if(this->getNumChannels() != upper.getNumChannels() || this->getChannels() != upper.getChannels())
        throw "not compatible";//we can only fusion equal devices
    Device ret(this);
    switch(type){
        case MAX:
        foreach (int channel, getChannels()) {
            ret.setChannel(channel,upper.getChannelValue(channel)>getChannelValue(channel) ? upper.getChannelValue(channel) : getChannelValue(channel));
        }
        break;
        case MIN:
        foreach (int channel, getChannels()) {
            ret.setChannel(channel,upper.getChannelValue(channel)<getChannelValue(channel) ? upper.getChannelValue(channel) : getChannelValue(channel));
        }
        break;
    default :
        break;
    }
}

int Device::getNumChannels()
{
    return dmxChannels.count();
}

QMap<int, float> Device::getChannelValues()
{
    return dmxChannels;
}

QList<int> Device::getChannels()const{
    return dmxChannels.keys();
}

void Device::setChannel(int channel, float value)
{
    dmxChannels.remove(channel);
    dmxChannels.insert(channel,value);
}

float Device::getChannelValue(int channel)
{
    return dmxChannels.value(channel);
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
    return other->getChannels() == getChannels();
}

bool Device::valuesEqual(Device other)
{
    return (getChannelValues() == getChannelValues());
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

QString Device::getDeviceId()
{
    return devId;
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
    return a.getChannels() == b.getChannels();
}

