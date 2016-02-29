#include "beamerdeviceprovider.h"
#include <QColor>

beamerDeviceProvider::beamerDeviceProvider(WebSocketServer *server, VirtualDeviceManager *manager) :
    WebSocketServerProvider(server), OutputDevice(),
    filterDevManager(manager)
{
    server->registerProvider(this);

    filterDevManager.addAcceptedType(Device::Beamer);
}

void beamerDeviceProvider::clientRegistered(QJsonObject msg, int id)
{
    qDebug() << "beamer registered : " << msg;
    if(msg.contains("deviceId")){
        QString devId = msg.value("deviceId").toString();

        QMap<QString, QSharedPointer<Device> > availableDevs = filterDevManager.getDevices();
        if(availableDevs.contains(devId)){
            devices.insert(id,availableDevs.value(devId).data()->getDeviceId());
        }
        else{
            qDebug() << "error: beamer device id did not match!";
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

void beamerDeviceProvider::publish(QMap<QString, QSharedPointer<DeviceState> > targetDevices, QMap<QString, QSharedPointer<DeviceState> > changes)
{
    foreach(QString devId, changes.keys()){
            QList<int> beamerIDs = devices.keys(devId);
            if(beamerIDs.length() == 0) continue; //no beamers registered for this device;
            QJsonObject msg;
            QColor c;
            QSharedPointer<ChannelDeviceState> d = changes.value(devId).dynamicCast<ChannelDeviceState>();
            int lowestChannel = d.data()->getFirstChannel();
            c.setRed(d.data()->getChannelValue(lowestChannel + 0)*255);
            c.setGreen(d.data()->getChannelValue(lowestChannel + 1)*255);
            c.setBlue(d.data()->getChannelValue(lowestChannel + 2)*255);
            msg.insert("color",c.name());
            c.setRed(d.data()->getChannelValue(lowestChannel + 3)*255);
            c.setGreen(d.data()->getChannelValue(lowestChannel + 4)*255);
            c.setBlue(d.data()->getChannelValue(lowestChannel + 5)*255);
            msg.insert("highlightedColor",c.name());
            foreach(int devId,beamerIDs)
                sendMsg(msg,devId);
    }
}
/*
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
*/


