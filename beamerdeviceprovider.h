#ifndef BEAMERDEVICEPROVIDER_H
#define BEAMERDEVICEPROVIDER_H

#include "websocketserver.h"
#include "websocketserverprovider.h"
#include "outputdevice.h"
#include <QMap>
#include <QList>
#include "device.h"
#include <beamerdevice.h>
class beamerDeviceProvider : public WebSocketServerProvider, public OutputDevice
{
public:
    beamerDeviceProvider(WebSocketServer *server, QList<Device> *availableVirtualDevicesP);
    void clientRegistered(QJsonObject msg,int clientIdCounter);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
    void publish(QList<Device> targetDevices, QList<Device> changes);
    void publishShutter(QList<Device> beamer);
    void publishShutter(BeamerDevice beamer, bool dismissDevID = false);
private:
    QList<Device> *availableVirtualDevices;
    QMap<int,Device> devices;
};

#endif // BEAMERDEVICEPROVIDER_H
