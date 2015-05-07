#include "beamerdeviceprovider.h"

beamerDeviceProvider::beamerDeviceProvider(WebSocketServer *server, QList<Device> availableVirtualDevicesP) : WebSocketServerProvider(server), OutputDevice(),
    availableVirtualDevices(availableVirtualDevicesP)
{
    server->registerProvider(this);
}

void beamerDeviceProvider::clientRegistered(QJsonObject msg, int id)
{
    qDebug() << "beamer registered : " << msg;
    if(msg.contains("deviceId")){
        QString devId = msg.value("deviceId").toString();

        foreach(Device d, availableVirtualDevices){
            if(d.getDeviceId() == devId){
                devices.insert(id,d);
            }
        }
    }
}

void beamerDeviceProvider::clientUnregistered(QJsonObject msg, int id)
{
    qDebug() << "beamerUnregistered";
    devices.remove(id);
}

void beamerDeviceProvider::clientMessage(QJsonObject msg, int id)
{
    qDebug() << "beamerMsg";
}

QString beamerDeviceProvider::getRequestType()
{
    return "asBeamer";
}

void beamerDeviceProvider::publish(QList<Device> targetDevices)
{
    foreach(Device d, targetDevices){
            int devID = devices.key(d,-1);
            if(devID  == -1) continue;
            QJsonObject msg;
            msg.insert("color","#0000ff");
            sendMsg(msg,devID);
    }
}


