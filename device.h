#ifndef DEVICE_H
#define DEVICE_H

#include <QList>
#include <QFile>
#include <QMap>
class Device
{
public:
    Device(QMap<int,float> channelsP, QString devIdP);
    Device(const Device &d);
    Device(const Device *d);
    enum FusionType { AV,MAX,MAXG,MIN,MING,OVERRIDE};
    Device fusionWith(Device upper, FusionType type, float opacity);
    int getNumChannels();
    QMap<int,float> getChannelValues();
    QList<int> getChannels() const;
    void setChannel(int channel,float value);
    float getChannelValue(int channel);
    static QList<Device> loadDevicesFromXml(QString file);
    bool deviceEqual(Device other);
    bool deviceEqual(Device *other);
    bool valuesEqual(Device other);
    Device findEqualDevice(QList<Device> devices);
    static QMap<int,float> toLagacy(QList<Device> devices);

    QDebug operator<<(QDebug debug);
    bool operator ==(const Device &other);
    QString getDeviceId();
private:
    QMap<int,float> dmxChannels;
    QString devId;

};
//QDebug operator<<(QDebug dbg, const Device &type);
bool operator==( const Device &a, const Device &b);
#endif // DEVICE_H
