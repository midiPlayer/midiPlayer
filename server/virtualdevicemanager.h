#ifndef VIRTUALDEVICEMANAGER_H
#define VIRTUALDEVICEMANAGER_H
#include <QObject>
#include <QMap>
#include "device.h"

class VirtualDeviceManager : public QObject
{
Q_OBJECT

public:
    VirtualDeviceManager();
    virtual QMap<QString,QSharedPointer<Device> > getDevices()  = 0;
signals:
    void virtualDevicesChanged();
private:
};

#endif // VIRTUALDEVICEMANAGER_H
