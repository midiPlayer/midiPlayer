#include "device.h"

Device::Device(QMap<int,float> channelsP)
{

}

Device Device::fusionWith(Device upper, Device::FusionType type, float opacity)
{
    if(this->getNumChannels() != upper.getNumChannels() || this->getChannels() != upper.getChannels())
        throw "not compatible";//we can only fusion equal devices
    Device ret(channels);
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
    return channels.count();
}

QMap<int, float> Device::getChannelValues()
{
    return channels;
}

QList<int> Device::getChannels()
{
    return channels.keys();
}

void Device::setChannel(int channel, float value)
{
    channels.remove(channel);
    channels.insert(channel,value);
}

float Device::getChannelValue(int channel)
{
    return channels.value(channel);
}

QMap<int, Device> Device::loadDevicesFromXml(QFile file)
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


