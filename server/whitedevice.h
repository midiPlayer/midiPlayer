#ifndef WHITEDEVICE_H
#define WHITEDEVICE_H
#include "device.h"
#include <QColor>
class WhiteDevice : public Device
{
public:
    WhiteDevice(int firstChannel, int numChannels, QString devIdP, QColor colorOfDeviceP = QColor(255,255,255),  QVector3D pos = QVector3D(0,0,0));
    QColor getColorOfDevice();
private:
    QColor colorOfDevice;
};

#endif // WHITEDEVICE_H
