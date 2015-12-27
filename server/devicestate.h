#ifndef DEVICESTATE_H
#define DEVICESTATE_H
#include <QMap>
#include "serializable.h"
class DeviceState : Serializable
{

public:
    enum FusionType { AV,MAX,MAXG,MIN,MING,OVERRIDE};
    DeviceState(QString devId, QMap<int,float>channels);
    DeviceState(QString devId);
    DeviceState(const DeviceState &state);
    DeviceState(DeviceState *state);
    DeviceState(QJsonObject serialized);
    QString deviceId;
    QMap<int,float> dmxChannels;
    DeviceState fusionWith(DeviceState upper, FusionType type, float opacity);
    void setChannel(int channel,float value);
    float getChannelValue(int channel);
    QList<int> getChannels() const;
    int getFirstChannel();
    QJsonObject serialize();
};

#endif // DEVICESTATE_H
