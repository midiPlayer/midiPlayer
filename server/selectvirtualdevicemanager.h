#ifndef SELECTVIRTUALDEVICEMANAGER_H
#define SELECTVIRTUALDEVICEMANAGER_H
#include "virtualdevicemanager.h"
#include "serializable.h"
#include <QObject>
#include "websocketserverprovider.h"

class SelectVirtualDeviceManager : public VirtualDeviceManager, public Serializable, public WebSocketServerProvider
{
Q_OBJECT

public:
    SelectVirtualDeviceManager(VirtualDeviceManager *input,WebSocketServer *ws,QJsonObject serialized = QJsonObject());
    QMap<QString,QSharedPointer<Device> > getDevices();

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();

    QJsonObject serialize();
    void deserialize(QJsonObject serialized);
public slots:
    void updateDevices();

private:
    QStringList selectedDevices;
    QList<Device::DeviceType> selectedTypes;

    VirtualDeviceManager *manager;
    QMap<QString, QSharedPointer<Device> > myDevs;
};

#endif // SELECTVIRTUALDEVICEMANAGER_H
