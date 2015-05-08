#ifndef DEVICE_H
#define DEVICE_H

#include <QList>
#include <QFile>
#include <QMap>
class Device
{
public:
    enum FusionType { AV,MAX,MAXG,MIN,MING,OVERRIDE};
    enum DeviceType{White,RGB,RGBW,Beamer};

    Device(QMap<int,float> channelsP, QString devIdP,DeviceType typeP);
    Device(const Device &d);
    Device(const Device *d);
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
    DeviceType getType();
private:
    QMap<int,float> dmxChannels;
    QString devId;
    DeviceType type;

};
//QDebug operator<<(QDebug dbg, const Device &type);
bool operator==( const Device &a, const Device &b);
#endif // DEVICE_H
