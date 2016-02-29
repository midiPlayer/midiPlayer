#ifndef FILTERVIRTUALDEVICEMANAGER_H
#define FILTERVIRTUALDEVICEMANAGER_H
#include "virtualdevicemanager.h"
class FilterVirtualDeviceManager : public VirtualDeviceManager
{
Q_OBJECT

public:
    FilterVirtualDeviceManager(VirtualDeviceManager *manager);

    QMap<QString,QSharedPointer<Device> > getDevices();
    void addAcceptedType(Device::DeviceType type);
public slots:
    void onVirtualDevsChanged();
private:
    QMap<QString,QSharedPointer<Device> > myDevs;
    VirtualDeviceManager *vDevManager;
    QList<Device::DeviceType> acceptedTypes;
};

#endif // FILTERVIRTUALDEVICEMANAGER_H
