#ifndef BEAMERDEVICEPROVIDER_H
#define BEAMERDEVICEPROVIDER_H

#include "websocketserver.h"
#include "websocketserverprovider.h"
#include "outputdevice.h"
#include <QMap>
#include <QList>
#include "device.h"
class beamerDeviceProvider : public WebSocketServerProvider, public OutputDevice
{
public:
    beamerDeviceProvider(WebSocketServer *server, QList<Device> availableVirtualDevicesP);
    void clientRegistered(QJsonObject msg,int id);
    void clientUnregistered(QJsonObject msg,int id);
    void clientMessage(QJsonObject msg,int id);
    QString getRequestType();
    void publish(QList<Device> targetDevices);
private:
    QList<Device> availableVirtualDevices;
    QMap<int,Device> devices;
};

#endif // BEAMERDEVICEPROVIDER_H
