#ifndef DEVICE_H
#define DEVICE_H

#include <QList>
#include <QFile>
#include <QMap>
#include <QVector3D>
#include "devicestate.h"

class Device
{
public:
    enum DeviceType{White,RGB,RGBW,Beamer};

    Device(QMap<int,float> channelsP, QString devIdP,DeviceType typeP,QVector3D pos = QVector3D(0,0,0));
    Device(int firstChannel, int numChannels, QString devIdP,DeviceType typeP,QVector3D pos = QVector3D(0,0,0));
    Device(const Device &d);
    Device(const Device *d);
    Device fusionWith(Device upper, DeviceState::FusionType type, float opacity);
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
    Device operator *(float percentage);
    QString getDeviceId() const;
    DeviceType getType();
    int getFirstChannel();
    QVector3D getPosition() const;
    void setPosition(const QVector3D &value);
    DeviceState getState();
    void setState(DeviceState stateP);

private:
    DeviceState state;
    QString devId;
    DeviceType type;
    QVector3D position;

};
//QDebug operator<<(QDebug dbg, const Device &type);
bool operator==( const Device &a, const Device &b);
#endif // DEVICE_H
