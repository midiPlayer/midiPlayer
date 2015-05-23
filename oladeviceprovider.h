#ifndef OLADEVICEPROVIDER_H
#define OLADEVICEPROVIDER_H
#include "outputdevice.h"
#include <ola/DmxBuffer.h>
#include <ola/io/SelectServer.h>
#include <ola/Logging.h>
#include <ola/client/ClientWrapper.h>
#include <ola/Callback.h>

class OlaDeviceProvider : public OutputDevice
{
public:
    OlaDeviceProvider();
    void publish(QList<Device> outDevices,QList<Device> changes);
private:
    bool active;
    ola::client::OlaClientWrapper wrapper;
    bool SendData();
};

#endif // OLADEVICEPROVIDER_H
