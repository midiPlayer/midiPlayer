#include "device.h"
#include <QDebug>
#include "devicestate.h"
#include <QJsonArray>

#define KEY_DEVICE_TYPE "device_type"
#define KEY_DEV_ID "device_id"


Device::Device(QString devIdP, Device::DeviceType typeP, QVector3D pos):
    devId(devIdP),type(typeP),position(pos)
{
}

Device::Device(QJsonObject serialized) : devId(serialized.value(KEY_DEV_ID).toString()),
    type(Device::Unknown)
{
    type = (DeviceType) serialized.value(KEY_DEVICE_TYPE).toInt();
}


/*QJsonArray Device::serializeDevices(QList<Device> devices)
{
    QJsonArray ret;
    foreach (Device dev, devices) {
        ret.append(dev.serialize());
    }
    return ret;
}*/

bool Device::deviceEqual(Device *other)
{
    return other->getDeviceId() == getDeviceId();
}



QString Device::getDeviceId() const
{
    return devId;
}

Device::DeviceType Device::getType()
{
    return type;
}

QVector3D Device::getPosition() const
{
    return position;
}

void Device::setPosition(const QVector3D &value)
{
    position = value;
}


QJsonObject Device::serialize()
{
    QJsonObject ret;

    ret.insert(KEY_DEVICE_TYPE,(int)type);

    ret.insert(KEY_DEV_ID,getDeviceId());
    return ret;
}


bool operator==(const Device &a,const Device &b)
{
    return a.getDeviceId() == b.getDeviceId();
}

