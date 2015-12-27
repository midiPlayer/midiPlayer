#include "devicestate.h"
#include <QDebug>
#include <QJsonArray>

#define KEY_DEVICESTATE_DEV_ID "devicestate_dev_id"
#define KEY_DEVICESTATE_CHANNELS "devicestate_channels"
#define KEY_DEVICESTATE_DMX_ID "devicestate_dmx_id"
#define KEY_DEVICESTATE_VALUE "devicestate_value"

DeviceState::DeviceState(QString devId, QMap<int, float> channels) : deviceId(devId), dmxChannels(channels)
{ }

DeviceState::DeviceState(QString devId) : deviceId(devId), dmxChannels()
{ }

DeviceState::DeviceState(const DeviceState &state) : deviceId(state.deviceId), dmxChannels(state.dmxChannels)
{ }

DeviceState::DeviceState(DeviceState *state) : deviceId(state->deviceId), dmxChannels(state->dmxChannels)
{ }

DeviceState::DeviceState(QJsonObject serialized) : deviceId(serialized.value(KEY_DEVICESTATE_DEV_ID).toString()),
    dmxChannels()
{
    foreach (QJsonValue value, serialized.value(KEY_DEVICESTATE_CHANNELS).toArray()) {
        dmxChannels.insert(value.toObject().value(KEY_DEVICESTATE_DMX_ID).toInt(),value.toObject().value(KEY_DEVICESTATE_VALUE).toDouble());
    }
}

DeviceState DeviceState::fusionWith(DeviceState upper, FusionType type, float opacity)
{
    DeviceState ret(this);
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
     case AV:
        foreach (int channel, getChannels()) {
            ret.setChannel(channel,upper.getChannelValue(channel)*opacity+getChannelValue(channel)*(1.0f-opacity));
        }

        break;

    default :
        qDebug() << "this Fusiontype is currently not implemented";
        break;
    }
    return ret;
}

void DeviceState::setChannel(int channel, float value)
{
    dmxChannels.remove(channel);
    dmxChannels.insert(channel,value);
}

float DeviceState::getChannelValue(int channel)
{
    return dmxChannels.value(channel);
}

QList<int> DeviceState::getChannels() const
{
    return dmxChannels.keys();
}

int DeviceState::getFirstChannel()
{
    return dmxChannels.keys().first();
}

QJsonObject DeviceState::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_DEVICESTATE_DEV_ID,deviceId);
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
