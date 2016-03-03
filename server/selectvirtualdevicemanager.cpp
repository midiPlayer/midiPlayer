#include "selectvirtualdevicemanager.h"
#include <QJsonArray>
#include "websocketserver.h"

#define KEY_ACCEPTED_DEVICES "acc_dev"
#define KEY_ACCEPTED_TYPES "acc_types"



SelectVirtualDeviceManager::SelectVirtualDeviceManager(VirtualDeviceManager *input, WebSocketServer *ws, QJsonObject serialized) :
    WebSocketServerProvider(ws), selectedDevices(), selectedTypes(), manager(input), myDevs()
{
    ws->registerProvider(this);

    connect(manager,SIGNAL(virtualDevicesChanged()),this,SLOT(updateDevices()));
    deserialize(serialized);
}

QMap<QString, QSharedPointer<Device> > SelectVirtualDeviceManager::getDevices()
{
    return myDevs;
}

void SelectVirtualDeviceManager::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject ret = serialize();
    QJsonArray devs;
    foreach(QString devId, manager->getDevices().keys()){
        QJsonObject devJson;
        devJson.insert("devId",devId);
        devJson.insert("type",(int)manager->getDevices().value(devId).data()->getType());
        devs.append(devJson);
    }
    ret.insert("devices",devs);
    sendMsg(ret,id,true);
}

void SelectVirtualDeviceManager::clientUnregistered(QJsonObject msg, int id)
{

}

void SelectVirtualDeviceManager::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("addAccepted")){
        QString devID = msg.value("addAccepted").toString();
        if(manager->getDevices().keys().contains(devID) && !selectedDevices.contains(devID))
            selectedDevices.append(devID);
    }
    if(msg.contains("rmAccepted")){
        selectedDevices.removeAll(msg.value("rmAccepted").toString());
    }
    if(msg.contains("addAccType")){
        Device::DeviceType type = (Device::DeviceType)msg.value("addAccType").toInt(-1);
        if(!selectedTypes.contains(type))
            selectedTypes.append(type);
    }
    if(msg.contains("rmAccType")){
        selectedTypes.removeAll((Device::DeviceType)msg.value("rmAccType").toInt(-1));
    }
    sendMsgButNotTo(msg,id,true);
}

QString SelectVirtualDeviceManager::getRequestType()
{
    return "selectVirtualDeviceManager";
}

void SelectVirtualDeviceManager::updateDevices()
{
    QMap<QString, QSharedPointer<Device> > avDev = manager->getDevices();
    bool changed = false;
    foreach (QString devId, avDev.keys()) {
        if(selectedDevices.contains(devId) || selectedTypes.contains(avDev.value(devId).data()->getType())){
            if(!myDevs.contains(devId)){
                myDevs.insert(devId,avDev.value(devId));
                changed = true;
            }
        }
        else{
            if(myDevs.contains(devId)){
                myDevs.remove(devId);
                changed = true;
            }
        }
    }
    if(changed)
        emit virtualDevicesChanged();
}

QJsonObject SelectVirtualDeviceManager::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_ACCEPTED_DEVICES, QJsonArray::fromStringList(selectedDevices));
    QJsonArray accTypes;
    foreach(Device::DeviceType type, selectedTypes){
        accTypes.append((int)type);
    }
    ret.insert(KEY_ACCEPTED_TYPES,accTypes);
    return ret;
}

void SelectVirtualDeviceManager::deserialize(QJsonObject serialized)
{
    foreach(QJsonValue v, serialized.value(KEY_ACCEPTED_DEVICES).toArray()){
        QString devId = v.toString();
        if(manager->getDevices().contains(devId))
            selectedDevices.append(devId);
    }
    foreach(QJsonValue v, serialized.value(KEY_ACCEPTED_TYPES).toArray()){
        int type = v.toInt(-1);
        if(type == -1)
            continue;
        selectedTypes.append((Device::DeviceType)type);
    }
}
