#include "channeldevicestate.h"
#include <QDebug>
#include <QJsonArray>

#include "channeldevice.h"
#include <QSharedPointer>

#define KEY_DEVICESTATE_DEV_ID "devicestate_dev_id"
#define KEY_DEVICESTATE_CHANNELS "devicestate_channels"
#define KEY_DEVICESTATE_DMX_ID "devicestate_dmx_id"
#define KEY_DEVICESTATE_VALUE "devicestate_value"


ChannelDeviceState::ChannelDeviceState(ChannelDevice *deviceP) : device(deviceP), dmxChannels()
{
    for(int i = device->getFirstChannel(); i < device->getFirstChannel() + device->getNumChannels();i++){
        dmxChannels.insert(i,0.0);
    }
}

ChannelDeviceState::ChannelDeviceState(const ChannelDeviceState &state) : device(state.device), dmxChannels(state.dmxChannels)
{ }

ChannelDeviceState::ChannelDeviceState(ChannelDeviceState *state) : device(state->device), dmxChannels(state->dmxChannels)
{ }

ChannelDeviceState::ChannelDeviceState(QJsonObject serialized,VirtualDeviceManager *manager) : dmxChannels()
{
    QMap<QString,QSharedPointer<Device> > available = manager->getDevices();
    QString devId = serialized.value(KEY_DEVICESTATE_DEV_ID).toString();
    if(!available.contains(devId))
        throw("no device found for given device id! Can't create device state");

    device = (available.value(devId).dynamicCast<ChannelDevice>().data());

    foreach (QJsonValue value, serialized.value(KEY_DEVICESTATE_CHANNELS).toArray()) {
        dmxChannels.insert(value.toObject().value(KEY_DEVICESTATE_DMX_ID).toInt(),value.toObject().value(KEY_DEVICESTATE_VALUE).toDouble());
    }
}

QSharedPointer<DeviceState> ChannelDeviceState::fusionWith(QSharedPointer<DeviceState> upperDeviceState, FusionType type, float opacity)
{
    QSharedPointer<ChannelDeviceState> upper = upperDeviceState.dynamicCast<ChannelDeviceState>();
    QSharedPointer<ChannelDeviceState> ret = QSharedPointer<ChannelDeviceState>(new ChannelDeviceState(this));
    if(upper.isNull() || upper.data()->getDevice()->getDeviceId() != getDevice()->getDeviceId() || this->getNumChannels() != upper.data()->getNumChannels() || this->getChannels() != upper.data()->getChannels())
        throw "not compatible";//we can only fusion equal devices
    switch(type){
     case MAX:
        foreach (int channel, getChannels()) {
            ret.data()->setChannel(channel,upper.data()->getChannelValue(channel)>getChannelValue(channel) ? upper.data()->getChannelValue(channel) : getChannelValue(channel));
        }
        break;
     case MIN:
        foreach (int channel, getChannels()) {
            ret.data()->setChannel(channel,upper.data()->getChannelValue(channel)<getChannelValue(channel) ? upper.data()->getChannelValue(channel) : getChannelValue(channel));
        }
        break;
     case AV:
        foreach (int channel, getChannels()) {
            ret.data()->setChannel(channel,upper.data()->getChannelValue(channel)*opacity+getChannelValue(channel)*(1.0f-opacity));
        }

        break;

    default :
        qDebug() << "this Fusiontype is currently not implemented";
        break;
    }
    return ret.dynamicCast<DeviceState>();
}

QSharedPointer<DeviceState> ChannelDeviceState::fusionAlone(DeviceState::FusionType type, float opacity)
{
    QSharedPointer<ChannelDeviceState> ret = copyToSharedPointer();
    if(type == DeviceState::AV){
        ret.data()->multiply(1-opacity);
    }
    return ret;
}

void ChannelDeviceState::setChannel(int channel, float value)
{
    dmxChannels.remove(channel);
    dmxChannels.insert(channel,value);
}

float ChannelDeviceState::getChannelValue(int channel)
{
    return dmxChannels.value(channel);
}

QList<int> ChannelDeviceState::getChannels() const
{
    return dmxChannels.keys();
}

int ChannelDeviceState::getNumChannels()
{
    return dmxChannels.size();
}

int ChannelDeviceState::getFirstChannel()
{
    return dmxChannels.keys().first();
}

QJsonObject ChannelDeviceState::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_DEVICESTATE_DEV_ID,getDevice()->getDeviceId());
    QJsonArray channelsJosn;
    foreach (int key, dmxChannels.keys()) {
        QJsonObject channelJson;
        channelJson.insert(KEY_DEVICESTATE_DMX_ID,key);
        channelJson.insert(KEY_DEVICESTATE_VALUE,dmxChannels.value(key));
        channelsJosn.append(channelJson);
    }
    ret.insert(KEY_DEVICESTATE_CHANNELS,channelsJosn);
    return ret;
}

QJsonArray ChannelDeviceState::getClientJson()
{
    QJsonArray channelsJosn;
    foreach (int key, dmxChannels.keys()) {
        channelsJosn.append(dmxChannels.value(key));
    }
    return channelsJosn;
}

void ChannelDeviceState::setClientJson(QJsonArray json)
{
    if(json.size() == dmxChannels.size()){
        for(int i = 0; i < dmxChannels.size() ; i++){
            setChannel(i+getFirstChannel(),json.at(i).toDouble());
        }
    }
    else{
        qDebug() << "ERROR (Devicestate): wrong channel count;";
    }
}

ChannelDeviceState ChannelDeviceState::operator *(float percentage)
{
    ChannelDeviceState ret = this->copy();
    ret.multiply(percentage);
    return ret;
}

void ChannelDeviceState::multiply(float percentage)
{
    foreach (int channel, getChannels()) {
        setChannel(channel,getChannelValue(channel)*percentage);
    }
}

ChannelDeviceState ChannelDeviceState::copy()
{
    return ChannelDeviceState(this);
}

QSharedPointer<ChannelDeviceState> ChannelDeviceState::copyToSharedPointer()
{
    return QSharedPointer<ChannelDeviceState>(new ChannelDeviceState(this));
}

bool ChannelDeviceState::equal(DeviceState *other)
{
    ChannelDeviceState *otherCDS = dynamic_cast<ChannelDeviceState*>(other);
    if(other == NULL)
        return false;
    return (getChannels() == otherCDS->getChannels());
}

void ChannelDeviceState::tryImport(ChannelDeviceState other)
{
    int numChannels = dmxChannels.size();
    int firstChannel = getFirstChannel();
    dmxChannels.clear();
    int count = 0;
    foreach (float v, other.dmxChannels.values()) {
        if(count >= numChannels)
            break;
        dmxChannels.insert(firstChannel+count,v);
        count ++;
    }
}

ChannelDevice *ChannelDeviceState::getDevice()
{
    return device;
}
