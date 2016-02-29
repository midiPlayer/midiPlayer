#ifndef OLADEVICEPROVIDER_H
#define OLADEVICEPROVIDER_H
#include "outputdevice.h"
#include <ola/DmxBuffer.h>
#include <ola/io/SelectServer.h>
#include <ola/Logging.h>
#include <ola/client/ClientWrapper.h>
#include <ola/Callback.h>
#include "filtervirtualdevicemanager.h"

class OlaDeviceProvider : public OutputDevice
{
public:
    OlaDeviceProvider(VirtualDeviceManager *manager);
    ~OlaDeviceProvider();
    void publish(QMap<QString, QSharedPointer<DeviceState> > outDevices,QMap<QString, QSharedPointer<DeviceState> > changes);
private:
    bool active;
    ola::client::OlaClientWrapper wrapper;
    bool SendData();
    FilterVirtualDeviceManager filterDevManager;
};

#endif // OLADEVICEPROVIDER_H
