#include "beamerdeviceprovider.h"
#include <QColor>

beamerDeviceProvider::beamerDeviceProvider(WebSocketServer *server, QList<Device> *availableVirtualDevicesP) : WebSocketServerProvider(server), OutputDevice(),
    availableVirtualDevices(availableVirtualDevicesP)
{
    server->registerProvider(this);

}

void beamerDeviceProvider::clientRegistered(QJsonObject msg, int id)
{
    qDebug() << "beamer registered : " << msg;
    if(msg.contains("deviceId")){
        QString devId = msg.value("deviceId").toString();

        foreach(Device d, *availableVirtualDevices){
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
            QList<int> devIDs = devices.keys(d);
            if(devIDs.length() == 0) continue;
            QJsonObject msg;
            QColor c;
            c.setRed(d.getChannelValue(0));
            c.setGreen(d.getChannelValue(1));
            c.setBlue(d.getChannelValue(2));
            msg.insert("color",c.name());
            c.setRed(d.getChannelValue(3));
            c.setGreen(d.getChannelValue(4));
            c.setBlue(d.getChannelValue(5));
            msg.insert("highlightedColor",c.name());
            foreach(int devId,devIDs)
                sendMsg(msg,devId);
    }
}

void beamerDeviceProvider::publishShutter(QList<Device> beamer)
{
    QJsonObject msg;
    foreach (Device d, beamer) {
        BeamerDevice b = static_cast<BeamerDevice>(b);
        publishShutter(b);
    }
}

void beamerDeviceProvider::publishShutter(BeamerDevice beamer, bool dismissDevID)
{
    QJsonObject msg;
    BeamerDevice::ShutterType shutterT = beamer.getShutterType();
      if(shutterT == BeamerDevice::CIRCULAR)
        msg.insert("shutterType","circular");
      else if(shutterT == BeamerDevice::NONE)
        msg.insert("shutterType","none");
      if(beamer.shutterParams.length() != 0)
          msg.insert("shutterParams",beamer.shutterParams);

      if(!dismissDevID){
          int devID = devices.key(beamer,-1);
          if(devID  == -1) return;
          sendMsg(msg,devID);
      }
      else
          sendMsg(msg);//sendToAll
}


