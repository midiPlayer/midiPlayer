#include "filtervirtualdevicemanager.h"

FilterVirtualDeviceManager::FilterVirtualDeviceManager(VirtualDeviceManager *manager) : vDevManager(manager),acceptedTypes()
{
    connect(manager,SIGNAL(virtualDevicesChanged()),this,SLOT(onVirtualDevsChanged()));
    onVirtualDevsChanged();
}

QMap<QString, QSharedPointer<Device> > FilterVirtualDeviceManager::getDevices()
{
    return myDevs;
}

void FilterVirtualDeviceManager::addAcceptedType(Device::DeviceType type)
{
    acceptedTypes.append(type);
    onVirtualDevsChanged();
}

void FilterVirtualDeviceManager::onVirtualDevsChanged()
{
    myDevs.clear();
    QMap<QString,QSharedPointer<Device> > availableDevs = vDevManager->getDevices();
    foreach(QString devKey, availableDevs.keys()){
        QSharedPointer<Device> dev = availableDevs.value(devKey);
        if(acceptedTypes.contains(dev.data()->getType())){
            myDevs.insert(devKey,dev);
        }
    }
    emit virtualDevicesChanged();
}
