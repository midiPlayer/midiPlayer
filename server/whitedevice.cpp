#include "whitedevice.h"
#include <QJsonObject>
#define KEY_COLOR_OF_DEVICE "device_color"

WhiteDevice::WhiteDevice(int firstChannel, int numChannels, QString devIdP,QColor colorOfDeviceP, QVector3D pos):
    ChannelDevice(firstChannel,numChannels,devIdP,Device::White,pos), colorOfDevice(colorOfDeviceP)
{
}

QJsonObject WhiteDevice::serialize()
{
    QJsonObject ret = ChannelDevice::serialize();
    ret.insert(KEY_COLOR_OF_DEVICE,colorOfDevice.name());
    return ret;
}

QColor WhiteDevice::getColorOfDevice()
{
    return colorOfDevice;
}
