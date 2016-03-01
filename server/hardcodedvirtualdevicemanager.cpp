#include "hardcodedvirtualdevicemanager.h"
#include "channeldevice.h"
#include "whitedevice.h"
#include "beamerdevice.h"
/*
 * Still the devices are memorized hard-coded in the programm.
 * If you change the number of devices at this point, every scene-qml-file has to be edited,
 * because there is a minColorNum for color changing...
 * ...so it might be very nice to introduce a user-friendly gui to configure the devices,
 * saved in a xml-file and parsed at the beginning!
 * This was an explanation in best Jonas' English...
 */


/*
 * So here is a first step:
 * You can now serialize and deserialize devices to JSON so it is easy to export them into a config file
 * This was an comment in Jakob's English...
 */

HardcodedVirtualDeviceManager::HardcodedVirtualDeviceManager() : devices()
{
    addDev(QSharedPointer<Device>(new ChannelDevice(0,4,"rgbw1",Device::RGBW,QVector3D(-2,-3,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(4,4,"rgbw2",Device::RGBW,QVector3D(-1,1,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(8,4,"rgbw3",Device::RGBW,QVector3D(2,-2,0))));
    addDev(QSharedPointer<Device>(new BeamerDevice(100,3,"beamer1")));
    addDev(QSharedPointer<Device>(new WhiteDevice(15,1,"White 1",QColor(255,0,0),QVector3D(3,0,0))));
}

QMap<QString, QSharedPointer<Device> > HardcodedVirtualDeviceManager::getDevices()
{
    return devices;
}

void HardcodedVirtualDeviceManager::addDev(QSharedPointer<Device> dev)
{
    devices.insert(dev.data()->getDeviceId(),dev);
    emit virtualDevicesChanged();
}
