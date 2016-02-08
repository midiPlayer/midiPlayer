#include "whitedevice.h"

WhiteDevice::WhiteDevice(int firstChannel, int numChannels, QString devIdP,QColor colorOfDeviceP, QVector3D pos):
    Device(firstChannel,numChannels,devIdP,Device::White,pos), colorOfDevice(colorOfDeviceP)
{
}

QColor WhiteDevice::getColorOfDevice()
{
    return colorOfDevice;
}
