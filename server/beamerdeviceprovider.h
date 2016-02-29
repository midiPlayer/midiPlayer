#ifndef BEAMERDEVICEPROVIDER_H
#define BEAMERDEVICEPROVIDER_H

#include "websocketserver.h"
#include "websocketserverprovider.h"
#include "outputdevice.h"
#include <QMap>
#include <QList>
#include "device.h"
#include <beamerdevice.h>
#include "virtualdevicemanager.h"
#include "filtervirtualdevicemanager.h"
class beamerDeviceProvider : public WebSocketServerProvider, public OutputDevice
{
public:
    beamerDeviceProvider(WebSocketServer *server, VirtualDeviceManager *manager);
    void clientRegistered(QJsonObject msg,int clientIdCounter);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
    void publish(QMap<QString, QSharedPointer<DeviceState> > targetDevices, QMap<QString, QSharedPointer<DeviceState> > changes);
    //void publishShutter(QList<Device> beamer);
    //void publishShutter(BeamerDevice beamer, bool dismissDevID = false);
private:
    FilterVirtualDeviceManager filterDevManager;
    QMap<int,QString > devices;
};

#endif // BEAMERDEVICEPROVIDER_H
