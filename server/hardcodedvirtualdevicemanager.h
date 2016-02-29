#ifndef HARDCODEDVIRTUALDEVICEMANAGER_H
#define HARDCODEDVIRTUALDEVICEMANAGER_H

#include "virtualdevicemanager.h"
#include <QSharedPointer>
#include "device.h"

class HardcodedVirtualDeviceManager : public VirtualDeviceManager
{
public:
    HardcodedVirtualDeviceManager();
    QMap<QString,QSharedPointer<Device> > getDevices();
private:
    QMap<QString, QSharedPointer<Device> > devices;
    void addDev(QSharedPointer<Device> dev);
};

#endif // HARDCODEDVIRTUALDEVICEMANAGER_H
