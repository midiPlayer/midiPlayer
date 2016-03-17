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
   // addDev(QSharedPointer<Device>(new ChannelDevice(0,4,"rgbw1",Device::RGBW,QVector3D(-2,-3,0))));
   // addDev(QSharedPointer<Device>(new ChannelDevice(4,4,"rgbw2",Device::RGBW,QVector3D(-1,1,0))));
   // addDev(QSharedPointer<Device>(new ChannelDevice(8,4,"rgbw3",Device::RGBW,QVector3D(2,-2,0))));
    addDev(QSharedPointer<Device>(new BeamerDevice(100,3,"beamer1")));

    addDev(QSharedPointer<Device>(new WhiteDevice(4,1,"t1w",QColor(255,255,255),QVector3D(1,0,0))));
    addDev(QSharedPointer<Device>(new WhiteDevice(2,1,"t2w",QColor(255,255,255),QVector3D(3,0,0))));
    addDev(QSharedPointer<Device>(new WhiteDevice(5,1,"t3w",QColor(255,255,255),QVector3D(5,0,0))));
    addDev(QSharedPointer<Device>(new WhiteDevice(1,1,"t4w",QColor(255,255,255),QVector3D(7,0,0))));
    addDev(QSharedPointer<Device>(new WhiteDevice(0,1,"t5w",QColor(255,255,255),QVector3D(9,0,0))));
    addDev(QSharedPointer<Device>(new WhiteDevice(3,1,"t6w",QColor(255,255,255),QVector3D(11,0,0))));

    addDev(QSharedPointer<Device>(new WhiteDevice(7,1,"h1-2w",QColor(255,255,255),QVector3D(11,0,0))));
    addDev(QSharedPointer<Device>(new WhiteDevice(9,1,"h3-4w",QColor(255,255,255),QVector3D(11,0,0))));
    addDev(QSharedPointer<Device>(new WhiteDevice(8,1,"h5-6w",QColor(255,255,255),QVector3D(11,0,0))));

    addDev(QSharedPointer<Device>(new WhiteDevice(10,1,"birne",QColor(255,255,255),QVector3D(11,0,0))));
    addDev(QSharedPointer<Device>(new WhiteDevice(6,1,"spot",QColor(255,255,255),QVector3D(11,0,0))));

    addDev(QSharedPointer<Device>(new ChannelDevice(32,4,"t1r",Device::RGBW,QVector3D(2,0,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(28,4,"t2r",Device::RGBW,QVector3D(4,0,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(24,4,"t3r",Device::RGBW,QVector3D(6,0,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(20,4,"t4r",Device::RGBW,QVector3D(8,0,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(16,4,"t5r",Device::RGBW,QVector3D(10,0,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(12,4,"t6r",Device::RGBW,QVector3D(12,0,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(36,4,"rechts_v",Device::RGBW,QVector3D(12,4,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(40,4,"rechts_h",Device::RGBW,QVector3D(12,6,0))));
    addDev(QSharedPointer<Device>(new ChannelDevice(44,4,"links",Device::RGBW,QVector3D(2,4,0))));




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
