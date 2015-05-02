#ifndef DEVICE_H
#define DEVICE_H

#include <QMap>
#include <QList>
#include <QFile>
class Device
{
public:
    Device(QMap<int,float> channelsP);
    Device fusionWith(Device upper, FusionType type, float opacity);
    enum FusionType { AV,MAX,MAXG,MIN,MING};
    int getNumChannels();
    QMap<int,float> getChannelValues();
    QList<int> getChannels();
    void setChannel(int channel,float value);
    float getChannelValue(int channel);
    static QMap<int,Device> loadDevicesFromXml(QFile file);

private:
    QMap<int,float> channels;

};

#endif // DEVICE_H
