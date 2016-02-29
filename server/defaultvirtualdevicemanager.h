#ifndef DEFAULTVIRTUALDEVICEMANAGER_H
#define DEFAULTVIRTUALDEVICEMANAGER_H
#include "virtualdevicemanager.h"
class DefaultVirtualDeviceManager : public VirtualDeviceManager
{
public:
    DefaultVirtualDeviceManager();
    QMap<QString,QSharedPointer<Device> > getDevices();
    void addDevice(QSharedPointer<Device> dev);
private:
    QMap<QString,QSharedPointer<Device> > devices;
};


#endif // DEFAULTVIRTUALDEVICEMANAGER_H
