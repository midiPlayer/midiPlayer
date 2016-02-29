#ifndef DEVICE_H
#define DEVICE_H

#include <QList>
#include <QFile>
#include <QMap>
#include <QVector3D>
#include "devicestate.h"
#include "serializable.h"

class ChannelDeviceState;

class Device : public Serializable
{
public:
    enum DeviceType{White,RGB,RGBW,Beamer,Unknown,MusicPlayer};

    Device(QString devIdP,DeviceType typeP,QVector3D pos = QVector3D(0,0,0));
    Device(QJsonObject serialized);
    //static QMap<QString, QSharedPointer<Device> > deserializeDevices(QJsonArray serialized);
    //static QJsonArray serializeDevices(QList<Device> devices);
    bool deviceEqual(Device *other);
    static QMap<int,float> toLagacy(QList<Device> devices);
    QString getDeviceId() const;
    DeviceType getType();
    QVector3D getPosition() const;
    void setPosition(const QVector3D &value);

    QJsonObject serialize();

    virtual QSharedPointer<DeviceState> createEmptyState() = 0;
private:
    QString devId;
    DeviceType type;
    QVector3D position;

};
//QDebug operator<<(QDebug dbg, const Device &type);
bool operator==( const Device &a, const Device &b);
#endif // DEVICE_H
