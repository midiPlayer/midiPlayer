#include "defaultvirtualdevicemanager.h"

DefaultVirtualDeviceManager::DefaultVirtualDeviceManager()
{
}

QMap<QString, QSharedPointer<Device> > DefaultVirtualDeviceManager::getDevices()
{
    return devices;
}

void DefaultVirtualDeviceManager::addDevice(QSharedPointer<Device> dev)
{
    if(devices.contains(dev.data()->getDeviceId()))
        throw("Cant add device! Device Id allready addeed!");
    devices.insert(dev.data()->getDeviceId(),dev);
}
